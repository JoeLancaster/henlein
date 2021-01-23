#ifndef DIR_UTILS_H
#define DIR_UTILS_H

#include <linux/limits.h>

typedef char PATH_STR_TYPE[PATH_MAX];

int is_dot_dirs (PATH_STR_TYPE);
int e_rp (const char *, PATH_STR_TYPE);
int is_dir (PATH_STR_TYPE);
int add_files_in_dir(struct dirent *, int file_cnt); 

#endif
