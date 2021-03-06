#include <stdio.h>
#include <string>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>
#include <dirent.h>
#include <sstream>
#include <iterator>

#include "Stack.hpp"
#include "Deque.hpp"

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
pthread_mutex_t lock;


using namespace std;
using std::string;

static Stack* clients_stack = create();         /* Clients stack */
static Deque* clients_deque = create_deque();   /* Clients deque */

/*
 * In this task we were based on tirgul number 6
 */

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

//based the code from tirgul 6: mutex.c
void *THREAD(void* input) {

    int *current_fd = (int *) input;
    int new_fd = *current_fd;

    //sleep(10);//to show a connection of several threads and not close quickly

    while(true){
        char buf[MAX_TEXT_SIZE];
        int numbytes;
        int err_flag = 0;
        if ((numbytes = recv(new_fd, buf, MAX_TEXT_SIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        string str_usr{buf};
        stringstream ss{str_usr};
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;

        vector<string> texts(begin, end);
        cout << texts[0] << endl;

        // Thread locks code section, to avoid race condition
        pthread_mutex_lock(&lock);
        if (texts[0] == "PUSH"/*PUSH <SOMETHING>*/){
            string word;
            for (int i = 1; i < texts.size(); ++i, word += ' ') {word += texts[i];}
            PUSH(clients_stack, word.c_str());
            print_stack(clients_stack);
        }
        else if (texts[0] == "POP"/*POP*/){
            POP(clients_stack, &err_flag);
            if (!err_flag){
                // Good response
                if(send(new_fd, "1", 1 ,0) == -1){
                    perror("send");
                }
            }
            else {
                // bad response back to user to present error
                if(send(new_fd, "0", 1 ,0) == -1){
                    perror("send");
                }
            }
        }
        else if (texts[0] == "TOP"/*TOP*/){
            char output[9] = "OUTPUT: ";
            char* top = TOP(clients_stack, &err_flag);
            char* out = strcat(output, top);
            if (!err_flag){
                if(send(new_fd, out, strlen(out),0) == -1){
                    perror("send");
                }
            }
            else {
                // bad response back to user to present error
                if(send(new_fd, "0", 1 ,0) == -1){
                    perror("send");
                }
            }

        }

        else if (texts[0] == "ENQUEUE"/*Enqueue <SOMETHING>*/){
            string word;
            for (int i = 1; i < texts.size(); ++i, word += ' ') {word += texts[i];}
            ENQUEUE(clients_deque, word.c_str());
            print_queue(clients_deque);
        }

        else if (texts[0] == "DEQUEUE"/*DEQUEUE*/){
            char output[9] = "OUTPUT: ";
            char* top = DEQUEUE_TAIL(clients_deque, &err_flag);
            char* out = strcat(output, top);
            if (!err_flag){
                if(send(new_fd, out, strlen(out),0) == -1){
                    perror("send");
                }
            }
            else {
                // bad response back to user to present error
                if(send(new_fd, "0", 1 ,0) == -1){
                    perror("send");
                }
            }

        }
        else if (texts[0] == "EXIT"){
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }
    //close(new_fd);
}

int main(void) {
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int create_thread;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *) &their_addr),
                          s, sizeof s);
        printf("server: got connection from %s\n", s);
        if (pthread_mutex_init(&lock, NULL) != 0) {
            printf("\n mutex init failed\n");
            return 1;
        }
        //pthread_t is the data type used to uniquely identify a thread
        pthread_t new_thread[10];
        int i = 0;
        //The pthread_create() function is used to create a new thread, with attributes specified by attr, within a process
        if(pthread_create(&new_thread[i++], NULL, THREAD, &new_fd) != 0){
            printf("Unable to create thread\n");
        }
        if (i >= BACKLOG) {
            i = 0;
            while (i < BACKLOG) {
                pthread_join(new_thread[i++], NULL);
            }
        }
        pthread_mutex_destroy(&lock);
    }
    close(sockfd);
    return 0;
}