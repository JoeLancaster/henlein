#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include "event.h"
#include "hen_action.h"

int main(int argc, char **argv) {
  hen_action action;
#ifdef DEBUG
  printf(".I changed this line.\n");
#endif
  printf("argc: %d\n", argc);
  if (argc < 2) {
    printf("FATAL: \n\tNo arguments given. I don't know what to do!\n");
    return -1;
  }
  int i;
  for(i = 1; i < argc - 1; i++) {
    action.file_name[i - 1] = argv[i];
  }
  printf("hello\n");
  action.cmd = argv[argc - 1];
  action.trigger = IN_CLOSE_WRITE;
  
  printf("Watching for: ");
  for(i = 0; i < argc - 2; i++) {
    printf("%s, ", action.file_name[i]);
  }
  printf("with command \"%s\"\n", action.cmd);
  for (i = 0; i < argc - 2; i++) {
    watch_init(action.file_name[i]);
  }
  for (;;) {
    watch_and_do(action);
  }
  return 0;
}
