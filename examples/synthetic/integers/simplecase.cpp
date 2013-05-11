__attribute__((annotate("kernel")))
int f(int *a, int b) {
    return 0;
}

int main(void) {

    int a = 2;

    int c = f(&a, 1);

    return c;

} 
