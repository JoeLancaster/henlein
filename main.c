#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

#include "event.h"
#include "hen_action.h"
#include "mask_names.h"
#include "timestamp.h"

const char *usage = "usage: %s [OPTIONS] file1 file2... command trigger\n"
  "\t-v: Verbose output\n"
  "\t-q: Quiet mode - only print the output of the command if it fails\n"
  "\t-h: Prints this message\n";

int main(int argc, char **argv) {
  hen_action action;
  size_t i;
  int verbose = 0;
  int opt;
  
  if (argc < 2) {
    fprintf(stderr, "No arguments given. I don't know what to do!\nSee -h for help\n");
    exit(EXIT_FAILURE);
  }

  while ((opt = getopt(argc, argv, "hvq")) != -1) {
    switch (opt) {
    case 'v':
      verbose = 1;
      break;
    case 'q': //share verbose variable on the assumption that their functionalities are disjoint, for now.
      verbose = -1;
      break;
    case 'h':
    default:
      fprintf(stderr, usage, argv[0]);
      exit(EXIT_FAILURE);
      break;
    }
  }
  //resolve arguments as filepaths
  for (i = 0; i < (size_t)(argc - optind - 2); i++) {
    size_t ind = i + optind;
    action.file_name[i] = malloc(PATH_MAX);
    char * ptr = realpath(argv[ind], action.file_name[i]);
    int eno = errno;
    if (eno != 0) {
      fprintf(stderr, "%s: %s\n", argv[ind], strerror(eno));
      exit(EXIT_FAILURE);
    } else if (ptr == NULL) {
      fprintf(stderr, "Problem locating file \"%s\"", argv[ind]);
      exit(EXIT_FAILURE);
    }
  }
 
  action.file_list_sz = argc - optind - 2;
  action.cmd = argv[optind + action.file_list_sz];
  uint32_t mask = string_to_mask(argv[argc - 1]);
  
  if (mask == 0) {
    fprintf(stderr, "\"%s\" is not a valid trigger\n", argv[argc - 1]);
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
    watch_and_do(action, wp_list, verbose);
  }
  free(wp_list);
  
  return 0;
}




