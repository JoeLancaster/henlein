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

const char *usage = "usage: %s [OPTIONS] file1 file2... command trigger\n"
  "\t-v: Verbose output\n"
  "\t-q: Quiet mode - only print the output of the command if it fails\n"
  "\t-h: Prints this message\n";

enum {
  FILE_MODE = 0,
  DIR_MODE,
  DIR_RECURSIVE_MODE
};

typedef char ARG_STR_TYPE[PATH_MAX];

#define DIRS_START_LEN 128

int is_dot_dirs (char *name) {
  int dotdot = strcmp(name, "..");
  int dot = strcmp(name, ".");
  return dotdot == 0 || dot == 0;
}

int e_rp (char * src, char * dest) {
  int eno = 0;
  errno = 0;
  realpath(src, dest);
  eno = errno;
  return eno;
}

int is_dir (const char *path) {
  struct stat s;
  if (stat(path, &s) != 0) {
    return 0;
  }
  return S_ISDIR(s.st_mode);
}

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
  //resolve arguments as filepaths
  //ARG_STR_TYPE *files;
  ARG_STR_TYPE tmp;
  //  int files_size = DIRS_START_LEN;
  /* if (directory_mode) { */
  /*   files = calloc(files_size, sizeof(ARG_STR_TYPE));     */
  /* } */
  int file_cnt = 0;
  for (i = 0; i < (size_t)(argc - optind - 2); i++) {
    size_t ind = i + optind;
    if (directory_mode == DIR_MODE) {
      int err = e_rp(argv[ind], tmp);
      if (err) {
	fprintf(stderr,"%s: %s", strerror(err), tmp);
	return -1;
      }
      if ( ! is_dir(tmp) ) {
	fprintf(stderr, "%s is_dir\n", tmp);
	continue;
      }
      DIR *d;
      struct dirent *dir;
      d = opendir(tmp);
      if (d == NULL) {
	fprintf(stderr, "Failed to open \"%s\"\n", tmp);
      } else {
	printf("Opened %s\n", tmp);
      }
      ARG_STR_TYPE dirname;
      strcpy(dirname, tmp);
      strcat(dirname, "/");
      if (d) {
	while ((dir = readdir(d)) != NULL) {
	  int isd = is_dir(dir -> d_name);
	  printf("dir -> d_name: %s\n", dir -> d_name);
	  if (isd && directory_mode == DIR_RECURSIVE_MODE) {
	    //go deeper!
	    printf("Deep.\n");
	  } else if ( ! isd && ! is_dot_dirs(dir -> d_name)) {
	    //files
	    /*
	      problem here is that dir -> d_name is DIR/name.xt but without DIR so realpath won't resolve it
	      so we need to build the name up from the directory
	      we have the dir name it's in tmp right now
	     */
	    ARG_STR_TYPE path;
	    strcpy(path, dirname);
	    strcat(path, dir -> d_name);
	    printf("path: %s\n", path);
	    err = e_rp(path, tmp);
	    if (err) {
	      fprintf(stderr, "Excluding \"%s\" because: %s\n", path, strerror(err));
	    } else {
	      fflush(stdout);
	      action.file_name[file_cnt] = malloc(PATH_MAX);
	      if (action.file_name[file_cnt] == NULL) {
		fprintf(stderr, "Malloc error.\n");
		exit(EXIT_FAILURE);
	      }
	      strcpy(action.file_name[file_cnt], tmp);
	      file_cnt++;
	    }
	  } else {
	    continue; //skip directories
	  }
	}
	closedir(d);
      }
    } else {
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




