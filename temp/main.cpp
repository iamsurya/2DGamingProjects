#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    std::cout<<"Pos\t"<<rand()%2000<<" "<<rand()%2000<<std::endl;
    std::cout<<"Speed\t"<<((rand()%100)+200)*((rand()%2) - 1 )<<" "<<((rand()%100)+200)*((rand()%2) - 1 )<<std::endl;
    return 0;
}