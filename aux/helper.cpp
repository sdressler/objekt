#include <iostream>

void printKernelVolumeSize(unsigned long long in, unsigned long long out) {
    
    std::cout << "\n*** KDV ESTIMATION ***\n";
    std::cout <<   "======================\n";
    std::cout << "IN    : " << in << "\tBytes\n";
    std::cout << "OUT   : " << out << "\tBytes\n\n";
    std::cout << "TOTAL : " << in + out << "\tBytes\n";
    std::cout <<   "======================\n\n";
}
