#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include "event.h"
#include "hen_action.h"

int main(int argc, char **argv) {
  hen_action action;
  printf("argc: %d\n", argc);
  if (argc < 2) {
    printf("FATAL: \n\tNo arguments given. I don't know what to do!\n");
    return -1;
  }
  int i;
  for (i = 1; i < argc - 1; i++) {
    action.file_name[i - 1] = argv[i];
  }
  action.file_list_sz = argc - 2;
  action.cmd = argv[argc - 1];
  action.trigger = IN_CLOSE_WRITE;
  
  watch_init1();
  
  printf("Watching for: ");
  for (i = 0; i < action.file_list_sz; i++) {
    printf("%s, ", action.file_name[i]);
  }
  printf("with command \"%s\"\n", action.cmd);
  wd_name_pair * wp_list;
  wp_list = add_watch(action);
  for (i = 0; i < action.file_list_sz; i++) {
    printf("File: \"%s\" has wd: %d\n", wp_list[i].file_name, wp_list[i].wd);
  }
  for (;;) {
    watch_and_do(action, wp_list);
  }
  return 0;
}
