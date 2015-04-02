#include <iostream>
#include <MOCA/A.h>
#include <MOCA/B.h>

using namespace std;

int main() {
    
    A *a = new A(10);
    a->print();
    
    B b(a);
    b.count();
    
    cout << a << endl;
    delete a;
    
    return EXIT_SUCCESS;
}
