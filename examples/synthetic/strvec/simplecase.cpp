#include <iostream>
#include <vector>
#include <string>

__attribute__((annotate("kernel")))
void add(std::vector<std::string> X) {
    for (std::vector<std::string>::iterator it = X.begin(); it != X.end(); ++it) {
        std::cout << *it << "\n";
    }
}

int main(void) {

    std::vector<std::string> X;

    X.push_back(std::string("This"));
    X.push_back(std::string("is"));
    X.push_back(std::string("a"));
    X.push_back(std::string("Test!"));

    std::cout << "Elems: " << X.size() << " Sizes: ";
    int s = 0;
    for (std::vector<std::string>::iterator it = X.begin(); it != X.end(); ++it) {
        s += it->size();
        std::cout << it->size() << " ";
    }
    std::cout << "\nTotal: " << s << "\n";
    add(X);

	return 0;

}

