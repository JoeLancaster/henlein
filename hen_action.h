#ifndef HEN_ACTION_H
#define HEN_ACTION_H

#include <stdint.h>
#include <stdlib.h>
#include <linux/limits.h>

#include "dir_utils.h"
#include "path.h"

#define FILES_MAX 65536

#ifdef FILE_LIST_DYNAMIC
typedef char * str_t;
#endif

typedef struct {
  #ifdef FILE_LIST_DYNAMIC
  char **file_name;
  #else
  char * file_name[FILES_MAX];
  #endif
  size_t file_list_sz;
  char * cmd; //absolute path to command
  uint32_t trigger; //IN_NOTIFY IN_CLOSE_WRITE etc....
} hen_action;

typedef struct {
  char * file_name;
  int wd;
} wd_name_pair;


int act_add_file(hen_action *, path_t *);
int build_from_dir(hen_action *, path_t *, int);

/* 
   wd appears to enumerate from 1 so we could get file names just with action.file_name[wd - 1], but don't know if that's defined behaviour
 */

#endif
