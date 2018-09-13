#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

#define TIMESTAMP_SIZE 11


void timestamp(char * msg, ...) {
  va_list ap;
  va_start(ap, msg);
  char out[TIMESTAMP_SIZE];
  
  time_t t = time(NULL);
  struct tm * tmp;
  tmp = localtime(&t);
  if (tmp == NULL) {
    perror("localtime");
    exit(-1);
  }

  if(strftime(out, sizeof(out),"[%H:%M:%S]" ,tmp) == 0) {
    fprintf(stderr, "strftime returned 0");
    exit(-1);
  }
  printf("%s | ", out);
  vprintf(msg, ap);
  va_end(ap);
  return;
}
