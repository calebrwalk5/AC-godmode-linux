#include <inttypes.h>

#include <stdio.h>

#include <stdlib.h>

int main(int argc, char * argv[]) {
  if (argc < 3) {
    printf("Incorrect usage of memedit\n", argv[0]);
    return 1;
  }

  char * endptr;
  unsigned long address = strtoul(argv[1], & endptr, 16);
  if ( * endptr != '\0') {
    printf("Invalid address: %s\n", argv[1]);
    return 1;
  }

  int value = atoi(argv[2]);

  int * ptr = (int * ) address;

  * ptr = value;

  printf("Value %d assigned to address 0x%lx\n", value, address);

  return 0;
}