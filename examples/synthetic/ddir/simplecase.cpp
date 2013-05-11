#include <iostream>

__attribute__((annotate("kernel")))
void f(int *in, int *out, int *inout, int M, int N) {

    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            out[i + j] = in[i] + in[j];
        }

        inout[i] = inout[j] + 1;

    }

}

int main(void) {

    int N = 100, M = 200;

    int *a = new int[M];
    int *b = new int[N + M];
    int *c = new int[M];

    std::cout << "MEASURED : " << sizeof(int) * (M + M + M + N + 2) << " Bytes\n";

    f(a, b, c, M, N);

    return 0;

}
