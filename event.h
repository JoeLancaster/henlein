#ifndef EVENT_H
#define EVENT_H

#include "hen_action.h"

#define MAX_WATCHED_FILES 10

void watch_init(void);
void watch_and_say(char *);
void watch_and_do(hen_action, wd_name_pair *);
wd_name_pair * add_watch(hen_action);

#endif //EVENT_H
