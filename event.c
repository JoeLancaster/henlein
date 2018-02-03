#include "event.h"
#ifdef __linux__

#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#endif //__linux__

int fd;

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
