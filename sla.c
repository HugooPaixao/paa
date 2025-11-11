#include <stdio.h>
#include <stdlib.h>

int main() {
  int *n = (int*) malloc (sizeof(int));
  printf("%d", *n);
  return 0;
}