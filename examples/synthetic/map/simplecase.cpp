#include <iostream>
#include <map>
#include <string>

class A {
public:
    int a;
    int b;

    int size() { return 2 * sizeof(int); }
};

__attribute__((annotate("kernel")))
void k(std::map<std::string, A> &m) {

//    A a = m["Test"];
//    std::cout << a.a << " " << a.b << "\n";

}

int main(void) {

    std::map<std::string, A> m;

    m.insert(std::make_pair("Foo", A()));
    m.insert(std::make_pair("Bar", A()));
    
    k(m);

    int size = 0;
    for (std::map<std::string, A>::iterator it = m.begin(); it != m.end(); ++it) {
        size += it->first.size() + it->second.size();
    }

    std::cout << "MEASURED: " << size << " Bytes\n";

	return 0;

}

