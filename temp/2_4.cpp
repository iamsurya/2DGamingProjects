#include <iostream>
#include <cstring>
#include <string>

class A{
public:
    A(const std::string & n) : name(n) {}
    ~A() {std::cout << "base" << std::endl;}

    virtual void f() {std::cout << "A::f" << std::endl;}
    void g() {std::cout << "A::g" << std::endl;}
private:
std::string name;
};

class B: public A {
    public:
        B(const std::string &n, const char * t) : A(n), title(new char[strlen(t) + 1])  {strcpy(title,t);}
        ~B() {delete [] title; std::cout << "derived" << std::endl;}
    void f() {std::cout << "B::f" << std::endl;}
    void g() {std::cout << "B::g" << std::endl;}
private:
char * title;
};

int main()
{
    B * x = new B("Thane", "Whiterun");
    x->f();
    x->g();
    delete x;
}