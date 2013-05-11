#include <iostream>
#include <vector>
#include <string>

class Z {
public:
    int a, b, c;

    std::vector<double> d;

    Z() {
        d.resize(10);
    }

    int getSize() {
        return 3 * sizeof(int) + d.size() * sizeof(double);
    }
};

template <class T>
class Y {
public:
    T t;
    std::vector<Z> z;

    Y() {
        z.resize(10);
    }

    int getSize() {
        //return 0;
        return z[0].getSize() * z.size();
    }
};

class X {
public:
    int a;
    int b;
    double c;
    double d;

    Y<std::string> y;

    int getSize() {
        return 2 * sizeof(int) + 2 * sizeof(double) + y.getSize();
    }
};

__attribute__((annotate("kernel")))
void add(X x) {

}

int main(void) {

    X x;

    std::cout << "Size: " << x.getSize() << "\n";
    add(x);

	return 0;

}

