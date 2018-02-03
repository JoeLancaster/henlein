#ifndef EVENT_H
#define EVENT_H

#ifdef __linux__

extern int fd;

#endif //__linux__

struct fd_name_pair {
  int fd;
  char * name;
  int name_len;
};
void watch_init(char *);
void watch_and_say(char *);
void watch_and_do(char *);


#endif //EVENT_H
