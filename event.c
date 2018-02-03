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
  int err_watch = inotify_add_watch(fd, file_name, IN_MODIFY);
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

void watch_and_do(char *file_name){
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
  printf("Executing make.\n");
  pid_t pid = fork();
  int status = 0;
  if (pid == -1) {
    exit(-1);
  } else if (pid > 0) {
    waitpid(pid, &status, 0);
  } else {
    //child
    char * const _argv[] = {"/usr/bin/make", NULL};
    //char * const envp[] = {getenv("PATH")};
    execve("/usr/bin/make", _argv,  environ);
  }
  printf("make exits with %d\n", status);
  return;
#endif //__linux__
}
