#include <iostream>

class X {
public:
    double *vals;
    X *next;

};

__attribute__((annotate("kernel")))
void f(X x) {

}

int main(void) {

    int N = 100;

    X x1, x2;

    x1.vals = new double[N];
    x2.vals = new double[N * N];

    x1.next = &x2;
    x2.next = NULL;

    std::cout << "Size: " << N * sizeof(double) << " Bytes\n";

    f(x1);

    return 0;

}
