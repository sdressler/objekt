#include <iostream>
#include <sstream>
#include "/home/bzcdress/repos/elapsd/include/elapsd/elapsd.h"
    
ENHANCE::elapsd e("/home/bzcdress/repos/papers/kdv/data/europar-new.db", "NEW");

class X {

    int x;
    int y;

};

__attribute__((annotate("kernel")))
void f(int *a, X *x) {

}
/*
void allocate_memory(int **first, X **third, int size) {
    
    *first = new int[size];
    *third = new X[size*size];

//    *fourth = new X*[size];


}
*/

int main(void) {
 
    e.addDevice(0, "CPU");

    int N = 0;

    for (int i = 0; i < 4; i++) {
        N += 1000;
        std::stringstream ss;
        ss << "Kernel " << i << " Elems: " << N * 3 << " Size: " << N * sizeof(int) * 3;
        e.addKernel(i, ss.str());
    }

    for (int i = 0; i < 4; i++) {

        int N = 1000 << i;
        
        int *a = new int[N];
        X *x = new X[N];
      
//        std::cout << "Size: " << N * sizeof(int) * 3 << "\n";
//        std::cout << "Elems: " << N * 3 << "\n";


        e.startTimer(i,0);
        f(a, x);
        e.stopTimer(i,0);

        delete[] x;
        delete[] a;

    }

    e.commitToDB();    

    return 0;

}
