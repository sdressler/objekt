#include <vector>
#include <string>
#include <iostream>

class A {
public:
    std::vector<A*> b;
    //A** b;
    std::string s;
    double x;
    
    int size() { 
        int vsize = 0;
        for (std::vector<A*>::iterator it = b.begin(); it != b.end(); ++it) {
            vsize += (*it)->size();
        }
        return sizeof(double) + s.size() + vsize;
    }

};

__attribute__((annotate("kernel")))
void f(A a) {

}

int main(void) {

    A a1, a2, a3;

    a1.s = "TEST";
    a2.s = "TEST";
    a3.s = "TEST";

    a1.b.push_back(&a2);
    a1.b.push_back(&a3);
 //   a1.b[0] = &a2;
 //   a1.b[1] = &a3;


//    A *a4 = a1->b[0];

    std::cout << "Size: " << a1.size() << " Byte\n";

    f(a1);

    return 0;

}
