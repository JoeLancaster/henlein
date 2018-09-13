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
#include <stdlib.h>

int fd;
extern char **environ;

void watch_init(char *file_name) {
  fd = inotify_init();
  int err_watch = inotify_add_watch(fd, file_name, IN_CLOSE_WRITE);
  if (fd < 0 || err_watch < 0) {
    perror("Initialization error");
    exit(-1);
  }
  printf("Watch descriptor: %d", err_watch);
  return;
}

void watch_init1() {
  fd = inotify_init();
  if (fd < 0) {
    perror("Initialization error");
    exit(-1);
  }
}

wd_name_pair * add_watch(hen_action action) {
  //build array of watch descriptors paired with file names
  wd_name_pair * wp_list = malloc(sizeof(wd_name_pair) * action.file_list_sz);
  int i;
  for (i = 0; i < action.file_list_sz; i++) {
    wd_name_pair wp;
    wp.file_name = action.file_name[i];
    wp.wd = inotify_add_watch(fd, action.file_name[i], action.trigger);
    if (wp.wd < 0) {
      perror("Error adding watch");
      exit(-1);
    }
    wp_list[i] = wp;
  }
  return wp_list;
}

void watch_and_do(hen_action action, wd_name_pair * wp_list) {
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
    int i = 0;
    while (wp_list[i++].wd != ev -> wd); //find which file wd refers to
    printf("wd index: %d\n", i);
    printf("File system event: %s. With cookie %u. With file %s\n", mask_to_string(ev -> mask), ev -> cookie, action.file_name[i - 1]);
    printf("Executing \"%s\"\n", action.cmd);
    pid_t pid = fork();
    int status = 0;
    if (pid == -1) {
      perror("Fork error:");
      exit(-1);
    }
    else if (pid > 0) { //parent
      waitpid(pid, &status, 0);
    } else {
      char * const _argv[] = {action.cmd, NULL};
      execve(action.cmd, _argv, environ);
    }
  }
  return;
}
