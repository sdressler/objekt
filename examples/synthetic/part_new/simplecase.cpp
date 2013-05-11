#include <iostream>

class X {
    public:
    int *x;
    double *y;

};

__attribute__((annotate("kernel")))
void f(X *x) {

}

void allocate_memory(X **x, int N) {

    (*x)->x = new int[N];
    (*x)->y = new double[N * N];    

}

int main(void) {

    int N = 100;
    X *x = new X();

    allocate_memory(&x, N);

    std::cout << "Size: " << N * sizeof(int) + N * N * sizeof(double) << " Bytes\n";

    f(x);

    return 0;

}
