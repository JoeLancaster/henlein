#define _GNU_SOURCE //needed for execvpe

#include <errno.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "timestamp.h"
#include "event.h"
#include "mask_names.h"
#include "dynamic_read.h"

#define BLK_SIZE 4096
#define READ_MAX (2 << 28) // 256MiB

int fd;

void watch_init() {
  fd = inotify_init(); //fd is unrelated to the file we are watching
  if (fd < 0) {
    perror("Initialization error");
    exit(EXIT_FAILURE);
  }
}

wd_name_pair * add_watch(hen_action action) {
  wd_name_pair *wp_list = malloc(sizeof(wd_name_pair) * action.file_list_sz);
  size_t i;
  for (i = 0; i < action.file_list_sz; i++) {
    wd_name_pair wp;
    wp.file_name = action.file_name[i];
    wp.wd = inotify_add_watch(fd, action.file_name[i], action.trigger);
    if (wp.wd < 0) {
      perror("Error adding watch");
      exit(EXIT_FAILURE);
    }
    wp_list[i] = wp;
  }
  return wp_list;
}

void watch_and_do(hen_action action, wd_name_pair * wp_list, const int verbose) {
  const size_t buf_size = sizeof(struct inotify_event) + NAME_MAX + 1; //should be PATH_MAX?
  char read_buf[buf_size] __attribute__ ((aligned(8)));
  struct inotify_event *ev;
  int read_bytes = read(fd, read_buf, buf_size);
  if (read_bytes <= 0) {
    perror("Inotify file descriptor error");
    exit(EXIT_FAILURE);
  }
  ev = (struct inotify_event *) read_buf;
  if (ev -> mask == action.trigger) {
    int io_p[2]; 
    int i = 0;
    while (wp_list[i++].wd != ev -> wd); //find which file wd refers to
    if(verbose > 0) {
      timestamp("File system event: %s. With file %s\n", mask_to_string(ev -> mask), action.file_name[i - 1]);
      timestamp("Executing \"%s\"\n", action.cmd);  
    }

    if (pipe(io_p) == -1) {
      perror("Pipe: "); 
      exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    int status = 0;
    if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (pid > 0) { //parent
      if (verbose == -1) { //quiet mode
	close(io_p[1]);
	int bread = 0;
	int pf;
	char *output = d_read(io_p[0], &bread, BLK_SIZE, READ_MAX, &pf);
	if (output == NULL) {
	  fprintf(stderr, "Lost all output from %s.\n", action.cmd);
	}
	waitpid(pid, &status, 0);
	if (status) {
	  printf("%.*s\n", bread, output);
	  free(output);
	}
	if (pf) {
	  fprintf(stderr, "Lost some output from %s.\n", action.cmd);
	}
      }
      if (verbose > 0) {
	timestamp("%s exits with %d\n", action.cmd, WEXITSTATUS(status));
      }
    } else {
      if (verbose == -1) {
	dup2(io_p[1], STDOUT_FILENO);
	dup2(io_p[1], STDERR_FILENO);
	close(io_p[0]);
	close(io_p[1]);
      }
      char * const _argv[] = {action.cmd, NULL};
      if (execvpe(action.cmd, _argv, environ) < 0) {
	perror("execvpe"); //
	exit(EXIT_FAILURE);
      }
    }
  }
  return;
}
