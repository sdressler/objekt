#include <iostream>

class X {

    int x;
    int y;

};

__attribute__((annotate("kernel")))
void f(int *a, double *b, X *x, X **y) {

}

void allocate_memory(int **first, double **second, X **third, int size) {
    
    *first = new int[size];
    *second = new double[size];

    *third = new X[size*size];

//    *fourth = new X*[size];


}

int main(void) {

    int N = 100;

    int *a;
    double *b;
    X *x, **y;
  
    y = new X*[N];

/*
    a = new int[N];
    b = new double[N];

    x = new X[N*N];
    y = new X[N];
*/

    allocate_memory(&a, &b, &x, N);

    std::cout << "Size: " << N * sizeof(int) + N * sizeof(double) + N * N * sizeof(X) + N * sizeof(X*) << " Bytes\n";

    f(a, b, x, y);

    return 0;

}
