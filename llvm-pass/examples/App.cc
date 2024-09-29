#include <iostream>
int fact(int n) { return n < 2 ? 1 : n * fact(n-1);}

int main() {
  std::cout << fact(10) + fact (5) << std::endl;
}