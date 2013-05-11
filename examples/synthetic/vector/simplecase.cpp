#include <iostream>
#include <vector>

__attribute__((annotate("kernel")))
void add(std::vector<double> &x, std::vector<double> &y, std::vector<double> &z) {
	for (int i = 0; i < 10; i++) {
		z[i] = x[i] + y[i];
	}

}

int main(void) {

	std::vector<double> x(10);
	std::vector<double> y(10);
	std::vector<double> z(10);
	
	std::vector<double>::iterator it;
	for (it = x.begin(); it != x.end(); ++it) {
		*it = 1;
	}
	for (it = y.begin(); it != y.end(); ++it) {
		*it = 2;
	}

    std::cout << "Elements: " << x.size() << " " << y.size() << " " << z.size() << "\n";
    std::cout << "Size: " << (x.size() + y.size() + z.size()) * sizeof(double) << " Byte \n";
	add(x, y, z);

	for (it = z.begin(); it != z.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	return 0;

}

