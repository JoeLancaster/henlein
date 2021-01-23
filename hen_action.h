#ifndef HEN_ACTION_H
#define HEN_ACTION_H

#include <stdint.h>
#include <stdlib.h>
#include <linux/limits.h>

#include "dir_utils.h"

#define FILES_MAX 2048

typedef struct {
  char * file_name[FILES_MAX];
  size_t file_list_sz;
  char * cmd; //absolute path to command
  uint32_t trigger; //IN_NOTIFY IN_CLOSE_WRITE etc....
} hen_action;

typedef struct {
  char * file_name;
  int wd;
} wd_name_pair;


int act_add_file(hen_action *, PATH_STR_TYPE);
int build_from_dir(PATH_STR_TYPE, hen_action *, int);

/* 
   wd appears to enumerate from 1 so we could get file names just with action.file_name[wd - 1], but don't know if that's defined behaviour
 */

#endif
