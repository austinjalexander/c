#include "stdio.h"

int main() {

  int a = 0, b = 1, c = 3;

  __asm__ ("add a, b, c");

  return 0;
}