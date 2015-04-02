#include "B.h"
#include "A.h"

B::B(A *a) {
    _a = a;
}

void B::count() {
    
    int max = _a->getN();
    for (int i = 0; i < max; i++)
        cout << "Okis #" << i + 1 << endl;
}
