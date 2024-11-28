#include <iostream>

class A {
public:
    void print() { std::cout << "A::print" << std::endl; }
};

class B {
public:
    void print() { std::cout << "B::print" << std::endl; }
};

class C : public A, public B {
public:
    //! If C does not override print() - compilation error due to ambiguity 
    //!void print() { std::cout << "C::print" << std::endl; } 
};

int main()
{
    C obj;
    obj.A::print();  // Calls A's version
    obj.B::print();  // Calls B's version
    obj.C::print();  // error: request for member 'print' is ambiguous
  //or obj.print();

    return 0;
}