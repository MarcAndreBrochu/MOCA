#ifndef B_H
#define B_H

#include <iostream>

using namespace std;

class A;
class B {
    
    public:
    B(A *a);
    void count();
    
    private:
    A *_a; // weak
};

#endif // B_H
