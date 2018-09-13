#ifndef HEN_ACTION_H
#define HEN_ACTION_H

#include <stdint.h>

typedef struct {
  char * file_name[10];
  char * cmd; //absolute path to command
  uint32_t trigger; //IN_NOTIFY IN_CLOSE_WRITE etc....
} hen_action;

#endif
