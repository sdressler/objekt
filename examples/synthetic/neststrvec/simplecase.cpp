#include <iostream>
#include <vector>
#include <string>
/*
class Z {
public:
    int a, b, c;

    std::vector<std::string> strs;

    Z() {
        strs.push_back("This");
        strs.push_back("is");
        strs.push_back("a");
        strs.push_back("Test!!!");
    }

    int size() {
        size_t s = 3 * sizeof(int);
        for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); ++it) {
            s += it->size();
        }
        return s;
    }
};

class Y {
public:
    int a;
    int b;
    double c;
    double d;

    Z z;

    int size() {
        return 2 * sizeof(int) + 2 * sizeof(double) + z.size();
    }
};
*/

class B {
public:
    std::string x;

    B() {
        x = "Test";
    }

};

class A : public std::vector<B> {
public:
    A() {
        resize(1);
    }
};

__attribute__((annotate("kernel")))
void add(A a) {

}

int main(void) {

    A a;

    a.size();

    add(a);

	return 0;

}

