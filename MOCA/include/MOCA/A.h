#ifndef A_H
#define A_H

#include <iostream>

using namespace std;

class A {
    
    public:
    A(int n);
    void print();
    
    int getN() { return _n; }
    
    private:
    int _n;
};

#endif // A_H
