#include <string>
#include <iostream>
#include <vector>
using std::string;

class Stack{
private:
    std::vector<string> _data{};
public:
    void PUSH(const string& text);
    string POP();
    string TOP() const;
    bool isEmpty() const;
    void clear();
    void print_stack();
};