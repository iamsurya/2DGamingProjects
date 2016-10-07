#include <iostream>

class string
{
public:
    string() {std::cout<<"Default "<< std::endl;}
    explicit string(const char *) {std::cout << "convert" << std::endl;}
    string(const string &) {std::cout << "copy" << std::endl;}
    ~string() {std::cout << "destructor" << std::endl;}
    string& operator=(const string &)
    {
        std::cout << "assign" << std::endl;
        return *this;
    }
};

int main()
{
    string X("hello"), Y = X;
    
return 0;
}