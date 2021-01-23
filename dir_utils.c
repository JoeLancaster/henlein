#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "dir_utils.h"
#include "hen_action.h"

int is_dot_dirs (PATH_STR_TYPE name) {
  int dotdot = strcmp(name, "..");
  int dot = strcmp(name, ".");
  return dotdot == 0 || dot == 0;
}

int e_rp (const char * src, PATH_STR_TYPE dest) {
  int eno = 0;
  errno = 0;
  realpath(src, dest);
  eno = errno;
  return eno;
}

int is_dir (PATH_STR_TYPE path) {
  struct stat s;
  if (stat(path, &s) != 0) {
    return 0;
  }
  return S_ISDIR(s.st_mode);
}

/*
  Returns number of files added, -1 indicates total failure
 */

int add_files_in_dir(PATH_STR_TYPE *files, hen_action *a, int num_files) {
  int our_count = 0;
  for (int i = 0; i < num_files; i++) {
    if ( act_add_file(a, files[i]) == 0 ) {
      our_count++;
    }
  }
  return our_count == 0 ? -1 : our_count;
}
