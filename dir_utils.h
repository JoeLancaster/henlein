#ifndef DIR_UTILS_H
#define DIR_UTILS_H

#include <linux/limits.h>


enum {
  FILE_MODE = 0,
  DIR_MODE,
  DIR_RECURSIVE_MODE
};

typedef char PATH_STR_TYPE[PATH_MAX + 1];

int is_dot_dirs (PATH_STR_TYPE);
int e_rp (const char *, PATH_STR_TYPE);
int is_dir (PATH_STR_TYPE);

#endif
