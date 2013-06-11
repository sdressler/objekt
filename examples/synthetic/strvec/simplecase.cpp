#include <iostream>
#include <vector>
#include <string>

#include "/home/bzcdress/repos/elapsd/include/elapsd/elapsd.h"

ENHANCE::elapsd e("/home/bzcdress/repos/papers/kdv/data/europar-strvec.db", "STRVEC");

__attribute__((annotate("kernel")))
void add(std::vector<std::string> X) {
    /*
    for (std::vector<std::string>::iterator it = X.begin(); it != X.end(); ++it) {
        std::cout << *it << "\n";
    }
    */
}

int main(void) {

    e.addDevice(0, "CPU");

    e.addKernel(0, "Kernel 0 Elems: 1000 Size: 3000");
    e.addKernel(1, "Kernel 0 Elems: 2000 Size: 6000");
    e.addKernel(2, "Kernel 0 Elems: 3000 Size: 9000");
    e.addKernel(3, "Kernel 0 Elems: 4000 Size: 12000");

    std::vector<std::string> X;

    for (int i = 2; i < 4; i++) {

        for (int j = 0; j < 1000; j++) {
            X.push_back(std::string("abc"));
        }
    
        std::cout << "Elems: " << X.size() << " Sizes: ";
        int s = 0;
        for (std::vector<std::string>::iterator it = X.begin(); it != X.end(); ++it) {
            s += it->size();
        }
        std::cout << s << "\n";

        e.startTimer(i, 0);
        add(X);
        e.stopTimer(i, 0);

    }

    e.commitToDB();

	return 0;

}

