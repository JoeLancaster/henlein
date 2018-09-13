#include "mask_names.h"
#include <sys/inotify.h>

char * mask_to_string(uint32_t m) {
  switch (m) {
  case IN_ACCESS:
    return "IN_ACCESS";
  case IN_ATTRIB:
    return "IN_ATTRIB";
  case IN_CLOSE_WRITE:
    return "IN_CLOSE_WRITE";
  case IN_CLOSE_NOWRITE:
    return "IN_CLOSE_NOWRITE";
  case IN_CREATE:
    return "IN_CREATE";
  case IN_DELETE:
    return "IN_DELETE";
  case IN_DELETE_SELF:
    return "IN_DELETE_SELF";
  case IN_MODIFY:
    return "IN_MODIFY";
  case IN_MOVE_SELF:
    return "IN_MOVE_SELF";
  case IN_MOVED_FROM:
    return "IN_MOVED_FROM";
  case IN_MOVED_TO:
    return "IN_MOVED_TO";
  case IN_OPEN:
    return "IN_OPEN";
  }
  return "ERR_UNKNOWN";
}
