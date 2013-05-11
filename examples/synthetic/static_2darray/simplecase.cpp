#include <iostream>

__attribute__((annotate("kernel")))
void f(const int box[][2]) {

}

int main(void) {

  int global_box[3][2] = { { 0, 1 }, { 0, 2 }, { 0, 3 } };

  std::cout << sizeof(int) * 3 * 2 << " Bytes\n";

  f(global_box);

  return 0;

}
