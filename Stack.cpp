#include "Stack.hpp"


void Stack::PUSH(const int &text) {
    _data.push_back(text);
}

string Stack::POP() {
    return _data.pop_back();
}

void Stack::TOP() const {
    cout << _data[_data.size() - 1] << endl;
}

bool Stack::isEmpty() const {
    return _data.size() == 0;
}

void Stack::print_stack() {
    for (int i = _data.size()-1; i >= 0; --i) {
        cout << _data[i] << endl;
    }
}