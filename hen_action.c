#include "hen_action.h"
#include "dir_utils.h"

/*
  Returns 0 if Ok, -1 if failure
*/
int act_add_file( hen_action *a, PATH_STR_TYPE path) {
  if (a -> file_list_sz == FILES_MAX) {
    return -1;
  }
  
  a -> file_name[a -> file_list_sz] = malloc(PATH_MAX);
  
  if (a -> file_name[a -> file_list_sz] == NULL) {
    return -1;
  } else {
    strcpy(a -> file_name[a -> file_list_sz], path);
    a -> file_list_sz++;
    return 0;
  }
}

void build_from_dir (PATH_STR_TYPE dirname, hen_action *a, int recursive) {
  
  int err = e_rp(argv[ind], tmp);
  if (err) {
    fprintf(stderr,"%s: %s", strerror(err), tmp);
    continue;
  }
      
  if ( ! is_dir(tmp) ) {
    fprintf(stderr, "Skipping %s. Not a directory.\n", tmp);
    continue;
  }
      
  DIR *d;
  struct dirent *dir;
  d = opendir(tmp);
  if (d == NULL) {
    fprintf(stderr, "Failed to open \"%s\"\n", tmp);
    continue;
  }
      
  PATH_STR_TYPE dirname;
  strcpy(dirname, tmp);
  strcat(dirname, "/");

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      //do you need to realpath a dir name like this? is it already absolute
      int isd = is_dir(dir -> d_name);
      printf("dir -> d_name: %s\n", dir -> d_name);
      if (isd && directory_mode == DIR_RECURSIVE_MODE) {
	//go deeper!
	printf("Deep.\n");
      } else if ( ! isd && ! is_dot_dirs(dir -> d_name)) {
	ARG_STR_TYPE path;
	strcpy(path, dirname);
	strcat(path, dir -> d_name);
	err = e_rp(path, tmp);
	if (err) {
	  fprintf(stderr, "Excluding \"%s\" because: %s\n", path, strerror(err));
	} else {
	  act_add_file( &action, path);
	  file_cnt++;
	}
      } else {
	continue; //skip directories
      }
    }
    closedir(d);
  }
}
