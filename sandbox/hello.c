#include <stdio.h>
#include <stdlib.h>

int main() {
  int sum = 0;
  int i = 11;
  while (--i) {
    if (i % 2 == 1) {
      sum += i;
    }
  }
  printf("%d\n", sum);
}
