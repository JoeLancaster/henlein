#include "event.h"
#include "mask_names.h"

#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int fd;
extern char **environ;

void watch_init(char *file_name){
  fd = inotify_init();
  int err_watch = inotify_add_watch(fd, file_name, IN_CLOSE_WRITE);
  if (fd < 0 || err_watch < 0) {
    perror("Initialization error");
    exit(-1);
  }
  return;
}

void watch_and_do(hen_action action) {
  const int buf_size = sizeof(struct inotify_event) + NAME_MAX +1;
  char read_buf[buf_size] __attribute__ ((aligned(8)));
  struct inotify_event *ev;
  int read_bytes = read(fd, read_buf, buf_size);
  if (read_bytes <= 0) {
    perror("Inotify file descriptor error");
    exit(-1);
  }
  ev = (struct inotify_event *) read_buf;
  if (ev -> mask == action.trigger) {
    printf("File system event: %s. With cookie %u. With file %s\n", mask_to_string(ev -> mask), ev -> cookie, action.file_name[0]); //asterisk
    //make less jargony
    printf("Executing \"%s\"\n", action.cmd);
    pid_t pid = fork();
    int status = 0;
    if (pid == -1) {
    perror("Fork error:");
    exit(-1);
    }
    else if (pid > 0) {
      //parent
      waitpid(pid, &status, 0);
    } else {
      char * const _argv[] = {action.cmd, NULL};
      execve(action.cmd, _argv, environ);
    }
  }
    return;
}
