#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#include "hen_action.h"
#include "dir_utils.h"
#include "path.h"

int act_add_file( hen_action *a, path_t *path) {
  if (a -> file_list_sz == FILES_MAX) {
    return -1;
  }
  
  a -> file_name[a -> file_list_sz] = malloc(PATH_MAX);
  
  if (a -> file_name[a -> file_list_sz] == NULL) {
    return -1;
  } else {
    strcpy( a -> file_name[a -> file_list_sz], path -> full );
    a -> file_list_sz++;
    return 0;
  }
}



//factor out recursive part

int build_from_dir (hen_action *a, path_t *path, int recursive) {
  int file_cnt = 0;
  DIR *d;
  struct dirent *dir;
  d = opendir(path -> full);
  if (d == NULL) {
    fprintf(stderr, "[d]Failed to open \"%s\"\n", path -> full);
    return 0;
  }
  path_deepen(path, "x"); //placeholder 
  
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (is_dot_dirs (dir -> d_name) ) {
	continue;
      }
      path_up(path);
      path_deepen(path, dir -> d_name);
      
      int isd = is_dir(path -> full);

      if (isd && recursive) {
	build_from_dir(a, path, 1);
	path_up(path);
		
      } else if ( ! isd ) {
	act_add_file(a, path);
	file_cnt++;
      } else {
	continue; //skip directories
      }
    }
    closedir(d);
  }
  return file_cnt;
}

/* int _build_from_dir(hen_action *a, path_t *p, int r) { */
/*   return _build_from_dir(a, p, r, 1); */
/* } */

