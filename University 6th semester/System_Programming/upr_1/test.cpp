#include <iostream>

int main()
{
    auto a  = +000001.5;

    std::cout << typeid(a).name() << std::endl;
    return 0;
}