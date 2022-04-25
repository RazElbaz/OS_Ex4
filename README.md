# OS_Ex4 Threads
The goal here in this homework is to understand how the OS implements malloc and free on
multi-threaded system and how we implement mutexes.


# Introduction:
The final submission will include a stack server.
In this homework you may only use TBB objects (Test&Set, Compare & Swap) for sync. You
must also implement your own multi-threaded malloc and free routines.


## Tasks:
1. You should name all source files *.cpp and not *.c so that the compiler will use g++ to
compile and not gcc. The reason for this is that the TBB library is written in c++ and not c.
However, “syntax wise”, there are no differences, and you can use regular c.
2. For your convenience – a c interface for compare and swap is attached at the end in
the appendix (so that you wont have to know c++ to use tbb). However, it isn’t that hard
and I encourage you to use the tbb (C++) interface directly, instead of mine (Don’t forget
that I can have bugs too).
3. Practice: Implement a multi-threaded lock free stack using POSIX methods. (For this
section only you may use conds, mutexes and the OS malloc) your stack must hold a variable
length null terminated strings. Your application will work from the command line initially
and network interface later and follow the following commands:
a. PUSH <text>\n
b. POP\n
c. TOP\n
4. The expected behaviour
a. PUSH will push <text> into the stack.
b. POP will pop text from the stack
c. TOP will display (output) the stack top.
d. Your output should begin with OUTPUT: prefix. These lines are the only lines that will
be checked by us. (Your only command that generate output is the TOP command)
e. For the sake of clarity and simplicity. You may assume that all commands are only in
uppercase. Text includes characters and can be capped at 1024 bytes. You may add
additional commands for operation or debugging. (Such as command for quitting, displaying
stack contents or clearing the stack.) Additionally, you may assume that all commands and
text end with a \n (ASCII 10) and that this character will not be part of the text.
Threads
f. You may assume all input is legal and that TOP or POP are only given when the stack is
in non-empty state but encouraged to give error message if you find illegal command. Use
ERROR: <cause> prefix for errors.
g. We will ignore any other prefix for output. However, you are encouraged to use
DEBUG: for debug output.
5. Start by implementing the stack so that it works with STDIN&STDOUT. (Single threaded
for now.)
a. As the final implementation will work with sockets you are encouraged to use
inheritance for inputs & outputs.
b. In this phase you can ignore syncing as you have only one thread conversing with the
user. (So no syncing is necessary.) However, it is advised to add synchronisation into the
code and start testing.
6. Practice: Merge the server implemented in ex 3 with the stack implemented in 5.
a. You now serve multiple client that transmit the stack command (instead of standard
input)
b. You must implement locking now as you may receive multiple commands to the stack
on different connections.
7. Practice: implement (using TBB) synchronisation primitives to replace the POSIX
primitive you used in 6.
8. Practice: implement new methods to manipulate the heap (malloc, free) in multi
threaded environment. Do not use POSIX primitives but the replacements you implemented
in 6.
You must also implement calloc to support threads (as pthread will call calloc) realloc is nice
to have but not required


##  Run:
1. make server
2. make client
3. run ./server
5. From another terminal open as many terminals as you want and type in ./client localhost that the user entered is printed on our server

## Pictures from the run:
  1.```server```
  
 ![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/server.png)
  
  2.```client```
  
 ![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/client.png)
  
  3. ```PUSH```
 
![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/push.png)
  
  4. ```TOP```
 
![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/top.png)
  
  5. ```POP```
 
![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/pop.png)
  
  6. ```Empty stack error```
 
![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/stackempty.png)
  
  7. ```Test```
 
![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/Test.png)
  
  8. ```multithread```
 
![alt text](https://github.com/RazElbaz/OS_Ex4/blob/main/Pictures%20from%20the%20run/multithread.png)
  
