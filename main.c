#include <stdio.h>
#include <string.h>

#include "event.h"


#define DEBUG
#define ARG_MAX 10

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("I changed this line\n");
#endif
  if (argc > ARG_MAX) {
    printf("FATAL: \n\tNo. of arguments exceeds ARG_MAX: %d\n", ARG_MAX);
    return -1;
  }
  if (argc < 2) {
    printf("FATAL: \n\tNo arguments given. I don't know what to do!\n");
    return -1;
  }
  watch_init(argv[1]);
  printf("Watching for %s\n", argv[1]);
  for (;;) {
    int stat = watch_and_do(argv[1], "/usr/bin/make");
    if (stat < 0){
      if (stat == -1) {
	printf("FATAL: \n\tError when opening attempting to read file.\n");
      } else if (stat == -2) {
	printf("FATAL: \n\tError when creating sub-process via fork\n");
      }
      return stat;
    }
  }
  return 0;
}
