#include <string.h>
#include <iostream>

class Student{
    public:
    Student(int x = 0) : name(new char[x+1]) {name[0] = '\0';}
    Student(const char * n) : name(new char[strlen(n)+1]) {strcpy(name,n);};
    Student(Student & s) : name(new char [strlen(s.name)+1]) {
        strcpy(name, s.name);}

    ~Student(){ delete [] name ;}

    void setname(const char *m)
    {
        delete [] name;
        name = new char [strlen(m)+1];
        strcpy(name, m);    
    }

    char * getname() const {return name;}

    Student& operator=(const Student & rhs)
    {
        if(this == & rhs) return * this;

        setname(rhs.name);

        return *this;

    }    

    private:
    char * name;
};

std::ostream& operator<<(std::ostream& out, const Student& stu) {
  return out << stu.getname();
}

int main()
{
    Student a("banana");
    std::cout<<a;
}

void print(const std::vector <int> & v)
{
    std::vector<int>::const_iterator ptr = vec.begin();

    while(ptr != vec.end())
    {
        cout<<*ptr;
        ptr++;
    }
}