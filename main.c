#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include "event.h"
#include "hen_action.h"


#define ARG_MAX 10

int main(int argc, char **argv) {
  hen_action action;
#ifdef DEBUG
  printf("I changed this line!\n");
#endif
  if (argc > ARG_MAX) {
    printf("FATAL: \n\tNo. of arguments exceeds ARG_MAX: %d\n", ARG_MAX);
    return -1;
  }
  if (argc < 2) {
    printf("FATAL: \n\tNo arguments given. I don't know what to do!\n");
    return -1;
  }

  action.file_name = argv[1];
  action.cmd = argv[2];
  action.trigger = IN_CLOSE_WRITE;
  watch_init(action.file_name);
  
  printf("Watching for %s\n", action.file_name);
  for (;;) {
    int stat = watch_and_do(action);
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
