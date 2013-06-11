#include <iostream>
#include <map>
#include <string>
/*
class A {
public:
    int a;
    int b;

    int size() { return 2 * sizeof(int); }
};
*/

#include "/home/bzcdress/repos/elapsd/include/elapsd/elapsd.h"

ENHANCE::elapsd e("/home/bzcdress/repos/papers/kdv/data/europar-map.db", "MAP");

__attribute__((annotate("kernel")))
void k(std::map<int, std::string> &m) {

//    A a = m["Test"];
//    std::cout << a.a << " " << a.b << "\n";

}

int main(void) {
    
    std::map<int, std::string> m;
/*

    m.insert(std::make_pair("Foo", A()));
    m.insert(std::make_pair("Bar", A()));
  */  

    e.addDevice(0, "CPU");

    e.addKernel(0, "Kernel 0 Elems: 1000 Size: 7000");
    e.addKernel(1, "Kernel 1 Elems: 2000 Size: 14000");
    e.addKernel(2, "Kernel 2 Elems: 3000 Size: 21000");
    e.addKernel(3, "Kernel 3 Elems: 4000 Size: 28000");

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 1000; j++) {
            m.insert(std::make_pair(j + i * 1000, "abc"));
        }

        e.startTimer(i, 0);
        k(m);
        e.stopTimer(i, 0);
    
        int size = 0;
        for (std::map<int, std::string>::iterator it = m.begin(); it != m.end(); ++it) {
            size += sizeof(int) + it->second.size();
        }
    
        std::cout << "MEASURED: " << size << " Bytes\n";

    }

    e.commitToDB();

	return 0;

}

