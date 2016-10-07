/* Learn how vector efficiency changes because of init lists, copy and convert constructors */
/* Learn what order a vector is copied in when size is doubled */
/* Learn how removing or adding the explicit keyword to a convert constructor does */

/* Create a class Number that has an int */
/* Create its default, copy and convert constructors */
/* Create destructor */
/* Add cout to these to make understanding easy */
/* Create a vector of numbers */
/* Use reserve syntax and see difference */

#include <iostream> // std::cout
#include <vector> // for vector

class Number
{
public:
    Number() : n(0) {
        std::cout<<"Default : "<<n<<std::endl;
    };
    
    explicit Number(int a) : n(a) {
        std::cout<<"Convert : "<<n<<std::endl;
    };
    Number(const Number &a) : n(a.n) {
        std::cout<<"Copy : "<<n<<std::endl;
    };

    operator=(const Number &rhs)
    {
        n = rhs.n;
        std::cout<<"assign : "<<n<<std::endl;
        return *this;
    }

/*    ~Number() {
        std::cout<<"destructor : "<<n<<std::endl; */
    }
private:
    int n;
};

int main()
{
    std::vector <Number> a(10);
//    a.reserve(2);
    for(int i = 0; i < 6; ++i)
    {
        a.push_back(i);
        std::cout<<"size : "<<a.size()<<" Cap : "<<a.capacity()<<std::endl;
    }
    return 0;
}
