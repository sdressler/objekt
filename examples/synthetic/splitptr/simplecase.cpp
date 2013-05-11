#include <iostream>

class X {

    int x;
    int y;

};

__attribute__((annotate("kernel")))
void f(X *x1, X *x2) {

}

int main(void) {

    int N = 100;

    X *x;

    x = new X[N];

/*
    a = new int[N];
    b = new double[N];

    x = new X[N*N];
    y = new X[N];
*/


    std::cout << "Size: " << N * sizeof(X) << " Bytes\n";

    f(&x[0], &x[N/2]);

    return 0;

}
