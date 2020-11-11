#if !defined(EVENT_H)
#define EVENT_H
/*each event is in a linked list and each one has a name and duration
only some have information*/
typedef struct event {
   char *name;
   int start_time, duration_minutes;
   void *info;
   struct event *next;
} Event;

#endif
