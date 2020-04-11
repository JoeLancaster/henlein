#ifndef HEN_ACTION_H
#define HEN_ACTION_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
  char * file_name[10];
  size_t file_list_sz;
  char * cmd; //absolute path to command
  uint32_t trigger; //IN_NOTIFY IN_CLOSE_WRITE etc....
} hen_action;

typedef struct {
  char * file_name;
  int wd;
} wd_name_pair;

/* 
   wd appears to enumerate from 1 so we could get file names just with action.file_name[wd - 1], but don't know if that's defined behaviour
 */

#endif
