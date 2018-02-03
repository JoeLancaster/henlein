#ifndef EVENT_H
#define EVENT_H

#ifdef __linux__

extern int fd;

#endif //__linux__

void watch_init(char *);
void watch_and_say(char *);

#endif //EVENT_H
