#include "event.h"
#ifdef __linux__

#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif //__linux__


#ifdef __linux__

int fd;
extern char **environ;

#endif


void watch_init(char *file_name){
  #ifdef __linux__
  fd = inotify_init();
  if (fd == -1) {
    exit(-1);
  }
  int err_watch = inotify_add_watch(fd, file_name, IN_CLOSE_WRITE);
  if (err_watch <= 0) {
    exit(-1);
  }
  return;
  #endif //__linux__
}

void watch_and_say(char *file_name){
#ifdef __linux__
  const int buf_size = sizeof(struct inotify_event) + NAME_MAX + 1;
  char read_buf[buf_size] __attribute__ ((aligned(8)));
  struct inotify_event *ev;
  int read_bytes = read(fd, read_buf, buf_size);
  if (read_bytes <= 0) {
    exit(-1);
  }
  ev = (struct inotify_event *) read_buf;
  printf("File system event: %d (IN_MODIFY) with file %s\n", ev -> mask, file_name);
  return;
#endif //__linux__
}

int watch_and_do(char *file_name, char * cmd){
#ifdef __linux__
  
  const int buf_size = sizeof(struct inotify_event) + NAME_MAX + 1;
  char read_buf[buf_size] __attribute__ ((aligned(8)));
  struct inotify_event *ev;
  int read_bytes = read(fd, read_buf, buf_size);
  if (read_bytes <= 0) {
    return -1;
  }
  //read inotify buffer
  
  ev = (struct inotify_event *) read_buf;
  printf("File system event: %u. With cookie %u. With file %s\n", ev -> mask, ev -> cookie, file_name);
  
  printf("Executing \"%s\"\n", cmd);
  pid_t pid = fork();
  int status = 0;
  if (pid == -1) {
    return -2;
  } else if (pid > 0) {
    printf("I'm the parent: %d\n", pid);
    waitpid(pid, &status, 0);
  } else {
    //child
    printf("I'm the child: %d\n", pid);
    char * const _argv[] = {cmd, NULL};
    execve(cmd, _argv,  environ);
    return 1;
  }
  printf("make exits with %d\n", status);
  return 1;
#endif //__linux__

}

/*void watch_and_do_many(fd_name_pair *){
#ifdef __linux__
  const int one_struct_size = sizeof(struct inotify_event) + NAME_MAX + 1;
  const int buf_size = MAX_WATCHED_FILES * one_struct_size;
  char read_buf[buf_size] __attribute__ ((aligned(8)));
  struct inotify_event *ev;
  int read_bytes = read(fd, read_buf, buf_size);
  if (read_byts <= 0) {
    exit(-1);
  }
  for (ev = (struct inotify_event *) (read_buf); read_buf <= read_buf + buf_size; read_buf += one_struct_size) {
    
  }
#endif//__linux__
}
*/
