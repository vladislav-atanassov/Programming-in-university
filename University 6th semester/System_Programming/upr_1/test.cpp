#include <iostream>

int main()
{
    auto a = 0X1234567Ul;

    std::cout << typeid(a).name() << std::endl;
    return 0;
}