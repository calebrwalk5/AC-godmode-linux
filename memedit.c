#include <stdio.h>

#include <stdlib.h>

#include <inttypes.h>

int main(int argc, char * argv[]) {
  if (argc < 3) {
    printf("Incorrect usage of memedit\n", argv[0]);
    return 1;
  }

  uintptr_t address = strtoull(argv[1], NULL, 16);
  int value = atoi(argv[2]);

  int * ptr = (int * ) address;

  * ptr = value;

  printf("Value %d assigned to address 0x%"
    PRIxPTR "\n", value, address);

  return 0;
}