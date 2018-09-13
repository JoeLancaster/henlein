#include "mask_names.h"
#include <stdint.h>
#include <sys/inotify.h>
#include <string.h>

uint32_t triggers[] = {IN_ACCESS, IN_ATTRIB, IN_CLOSE_WRITE, IN_CLOSE_NOWRITE, IN_CREATE, IN_DELETE, IN_DELETE_SELF, IN_MODIFY, IN_MOVE_SELF, IN_MOVED_FROM, IN_MOVED_TO, IN_OPEN, IN_MOVE, IN_CLOSE, IN_DONT_FOLLOW, IN_EXCL_UNLINK, IN_MASK_ADD, IN_ONESHOT, IN_ONLYDIR, IN_IGNORED, IN_ISDIR, IN_Q_OVERFLOW, IN_UNMOUNT, 0};

char * names[] = {"IN_ACCESS", "IN_ATTRIB", "IN_CLOSE_WRITE", "IN_CLOSE_NOWRITE", "IN_CREATE", "IN_DELETE", "IN_DELETE_SELF", "IN_MODIFY", "IN_MOVE_SELF", "IN_MOVED_FROM", "IN_MOVED_TO", "IN_OPEN", "IN_MOVE", "IN_CLOSE", "IN_DONT_FOLLOW", "IN_EXCL_UNLINK", "IN_MASK_ADD", "IN_ONESHOT", "IN_ONLYDIR", "IN_IGNORED", "IN_ISDIR", "IN_Q_OVERFLOW", "IN_UNMOUNT", NULL};

char * mask_to_string(uint32_t m) {
  int i = 0;
  while(triggers[i++] != m);
  return names[i];
}

uint32_t string_to_mask(char * n) {
  int i = 0;
  while(names[i] != NULL) {
    if(strcmp(names[i], n) == 0) {
      break;
    }
    i++;
  }
  return triggers[i];
}
