#include <stdio.h>

#include "event.h"

//test file change



int main(int argc, char **argv) {
  if (argc < 2) {
    return -1;
  }
  watch_init(argv[1]);
  printf("Watching for %s\n", argv[1]);
  for (;;) {
    watch_and_say(argv[1]);
  }
  return 0;
}
