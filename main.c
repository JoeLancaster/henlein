#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "event.h"
#include "hen_action.h"
#include "mask_names.h"
#include "timestamp.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("FATAL: \n\tNo arguments given. I don't know what to do!\n");
    return -1;
  }
  
  hen_action action;
  int i;


  //resolve arguments as filepaths
  for (i = 1; i < argc - 2; i++) {
    action.file_name[i - 1] = malloc(PATH_MAX);
    char * ptr = realpath(argv[i], action.file_name[i - 1]);
    if (errno != 0) {
      printf(strerror(errno));
      exit(EXIT_FAILURE);
    } else if (ptr == NULL) {
      printf("Problem locating file \"%s\"", argv[i]);
      exit(EXIT_FAILURE);
    }
  }
 
  action.file_list_sz = argc - 3;
  action.cmd = argv[argc - 2];
  uint32_t mask = string_to_mask(argv[argc - 1]);
  
  if (mask == 0) {
    printf("\"%s\" is not a valid trigger\n", argv[argc - 1]);
    exit(EXIT_FAILURE);
  }
  action.trigger = mask;
  
  watch_init();
  
  timestamp("Watching for: ");
  for (i = 0; i < action.file_list_sz; i++) {
    printf("%s, ", action.file_name[i]);
  }
  printf("with command \"%s\" on event: %s.\n", action.cmd, mask_to_string(action.trigger));
  wd_name_pair *wp_list;
  wp_list = add_watch(action);
  for (;;) {
        watch_and_do(action, wp_list);
  }
  free(wp_list);  
  return 0;
}




