#include <iostream>

class A {
public:
    void show() { std::cout << "A::show" << std::endl; }
};

class B : public A { };
class C : public A { };

class D : public B, public C { };  // Multiple inheritance

int main()
{
    D obj;
    obj.show();     //! error: request for member 'print' is ambiguous
    obj.B::show();  // The call has to be explicit 
    // or obj.C::show();
}