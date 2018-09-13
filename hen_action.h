#ifndef HEN_ACTION_H
#define HEN_ACTION_H

#include <stdint.h>
#include <stdlib.h>

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

#endif
