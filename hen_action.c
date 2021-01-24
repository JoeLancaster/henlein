#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#include "hen_action.h"
#include "dir_utils.h"


/*
  Returns 0 if Ok, -1 if failure
*/



/* int __build_from_dir (PATH_STR_TYPE full_path, hen_action *a, int count) { */
/*   int err = 0; */
/*   int file_cnt = 0; */
/*   DIR *d;   */
/*   struct dirent *dir; */
/*   d = opendir(full_path); */
/*   if (d == NULL) { */
/*     fprintf(stderr, "Failed to open \"%s\"\n", full_path); */
/*     return 0; */
/*   } */

/*   if (d) { */
/*     while ((dir = readdir(d)) != NULL) { */
/*       PATH_STR_TYPE path; */
/*       strcpy(path, full_path); */
/*       strcat(path, dir -> d_name); */
/*       if (is_dot_dirs(dir -> d_name)) { */
/* 	continue; */
/*       } */
/*       if (err) { */
/* 	fprintf(stderr, "Excluding \"%s\" because: %s\n", path, strerror(err)); */
/* 	continue; */
/*       } */
/*       int isd = is_dir(path); */
/*       if (isd) { */
/* 	//go deeper! */
/* 	//build_from_dir is given a path and then strcats it again */
/* 	strcat(path, "/"); */
/* 	return  __build_from_dir(path, a, count+file_cnt); */
/*       } else if ( ! isd && ! is_dot_dirs(dir -> d_name)) { */
/* 	act_add_file(a, path); */
/* 	file_cnt++; */

/*       } else { */
/* 	continue; //skip directories */
/*       } */
/*     } */
/*     closedir(d); */
/*   } */
/*   return file_cnt; */
/* } */


int act_add_file( hen_action *a, PATH_STR_TYPE path) {
  if (a -> file_list_sz == FILES_MAX) {
    return -1;
  }
  
  a -> file_name[a -> file_list_sz] = malloc(PATH_MAX);
  
  if (a -> file_name[a -> file_list_sz] == NULL) {
    return -1;
  } else {
    strcpy(a -> file_name[a -> file_list_sz], path);
    a -> file_list_sz++;
    return 0;
  }
}

int build_from_dir (hen_action *a, PATH_STR_TYPE dirname, int recursive) {
  a -> file_list_sz++;
  /* char * dirname = malloc(4096); */
  /* strcpy(dirname, tmp); */
  /* strcat(dirname, "/"); */

  int file_cnt = 0;
  DIR *d;
  struct dirent *dir;
  d = opendir(dirname);
  if (d == NULL) {
    fprintf(stderr, "Failed to open \"%s\"\n", dirname);
    return 0;
  }
  
  if (d) {
    char *path = malloc(PATH_MAX);
    while ((dir = readdir(d)) != NULL) {
      if (is_dot_dirs (dir -> d_name) ) {
	continue;
      }

      strcpy(path, dirname);
      strcat(path, dir -> d_name);
      printf("dirname: %s\npath: %s\n", dirname, path);

      int isd = is_dir(path);
      if (isd && recursive) {
	strcat(path, "/");
	build_from_dir(a, path, 1);
      } else if ( ! isd ) {
	act_add_file(a, path);
	file_cnt++;
      } else {
	continue; //skip directories
      }
    }
    closedir(d);
    free(path);
  }
  return file_cnt;
}

