#include "Stack.hpp"
using namespace std;

void Stack::PUSH(const string &text) {
    _data.push_back(text);
}

string Stack::POP() {
    if (isEmpty()) {throw invalid_argument("ERROR: Stack is empty, thus cannot pop!");}
    string top = _data[_data.size() - 1];
    _data.pop_back();
    return top;
}

string Stack::TOP() const {
    if (isEmpty()) {throw invalid_argument("ERROR: Stack is empty, thus cannot top!");}
    return "OUTPUT: " + _data[_data.size() - 1];
}

bool Stack::isEmpty() const {
    return _data.empty();
}

void Stack::print_stack() {
    if (isEmpty()) return;
    for (unsigned int i = _data.size()-1; i >= 0; --i) {
        cout << _data[i] << endl;
        if (i == 0) {break;}
    }
}

void Stack::clear() {
    _data.clear();
}
