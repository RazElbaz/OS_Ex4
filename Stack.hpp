#include <vector>
using std::string;

class Stack{
private:
    vector<string> _data{};
public:
    Stack(int capacity);
    void PUSH(const string& text);
    string POP();
    void TOP() const;
    bool isEmpty() const;
};