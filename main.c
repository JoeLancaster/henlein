#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>

#include "event.h"
#include "hen_action.h"
#include "mask_names.h"
#include "timestamp.h"
#include "dir_utils.h"

const char *usage = "usage: %s [OPTIONS] file1 file2... command trigger\n"
  "\t-v: Verbose output\n"
  "\t-q: Quiet mode - only print the output of the command if it fails\n"
  "\t-d: Directory mode: register all files in the given directory(ies)\n"
  "\t-h: Prints this message\n";

#define DIRS_START_LEN 128

int main(int argc, char **argv) {
  hen_action action;
  size_t i;
  int verbose = 0;
  int opt;
  int directory_mode = FILE_MODE;
  
  if (argc < 2) {
    fprintf(stderr, "No arguments given. I don't know what to do!\nSee -h for help\n");
    exit(EXIT_FAILURE);
  }

  while ((opt = getopt(argc, argv, "Ddhvq")) != -1) {
    switch (opt) {
    case 'd':
      directory_mode = DIR_MODE;
      break;
    case 'D':
      directory_mode = DIR_RECURSIVE_MODE;
      break;
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
  
  int file_cnt = 0;
  for (i = 0; i < (size_t)(argc - optind - 2); i++) {
    size_t ind = i + optind;
    PATH_STR_TYPE path;
    int err = e_rp(argv[ind], path);
    if (err) {
      fprintf(stderr, "%s: %s\n", argv[ind], strerror(err));
      continue;
    }
    if (directory_mode) {
      file_cnt += build_from_dir(path, &action, directory_mode == DIR_RECURSIVE_MODE);
    } else {
      act_add_file( &action, path);
      file_cnt++;
    }
  }
  printf("Watching %d files\n", file_cnt);
  printf("file_list_sz: %lu\n", action.file_list_sz);
  //action.file_list_sz = (argc - optind - 2);
  action.cmd = argv[optind + (argc - optind - 2)];
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




