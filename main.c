#include <stdio.h>
#include <string.h>


#include "event.h"

//test file change

#define ARG_MAX 10

int main(int argc, char **argv) {
  printf("I added this line.\n");
  if (argc > ARG_MAX || argc < 2) {
    return -1;
  }
  watch_init(argv[1]);
  printf("Watching for %s\n", argv[1]);
  for (;;) {
    watch_and_do(argv[1]);
  }
  return 0;
}
