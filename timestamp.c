#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

#define TIMESTAMP_STR "[%H:%M:%S]"
#define TIMESTAMP_SIZE sizeof(TIMESTAMP_STR)

void timestamp(char * msg, ...) {
  va_list ap;
  va_start(ap, msg);
  char out[TIMESTAMP_SIZE];
  
  time_t t = time(NULL);
  struct tm * tmp;
  tmp = localtime(&t);
  if (tmp == NULL) {
    perror("localtime");
    exit(EXIT_FAILURE);
  }

  if(strftime(out, sizeof(out), TIMESTAMP_STR, tmp) == 0) {
    fprintf(stderr, "strftime returned 0");
    exit(EXIT_FAILURE);
  }
  printf("%s | ", out);
  vprintf(msg, ap);
  va_end(ap);
  return;
}
