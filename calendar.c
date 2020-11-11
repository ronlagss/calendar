#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"
#include "event.h"
#define ONE 1


int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {
  Calendar *new_calendar;

  if(name == NULL || days < 1) {
    return FAILURE;
  }

  new_calendar = (Calendar*)malloc(sizeof(Calendar)); /*make calendar struct*/
  new_calendar->name = (char*)calloc(ONE, strlen(name)+ONE); /*dynamically allocate name*/
  new_calendar->events = (Event**)calloc(days, sizeof(Event*)); /*dynamically allocates array of events*/

  /*initialize all parameters for the struct*/
  new_calendar->comp_func = comp_func;
  new_calendar->free_info_func = free_info_func;

  new_calendar->days = days;
  new_calendar->total_events = 0;
  strcpy(new_calendar->name,name);

  /*initialized calendar parameter*/
  (*calendar) = new_calendar;

  /*null cases*/
  if(calendar == NULL || *calendar == NULL || (*calendar)->name == NULL ) { /*checks for memory allocation fail or null*/
    return FAILURE;
  }


  return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
  int i;
  Event *head;

  if( calendar == NULL || output_stream == NULL) { /*does calendar have to have a * */
    return FAILURE;
  }

  /*if print_all is 1 everything is shown*/
  if(print_all == 1) {
    fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
    fprintf(output_stream, "Days: %d\n", calendar->days);
    fprintf(output_stream, "Total Events: %d\n", calendar->total_events);
    fprintf(output_stream, "\n");
    fprintf(output_stream, "**** Events ****\n");

  } else { /*otherwise just events is shown*/
    fprintf(output_stream, "**** Events ****\n");

  }

  /*total events have to be greater than 0 in order for it to print out any days*/
  if(calendar->total_events > 0 ) {

    for(i = 0; i < calendar->days ; i++) {
      fprintf(output_stream,"Day %d\n",i + 1); /*shows days*/

      head= calendar->events[i];

      while(head != NULL) { /*head will keep going until reaches end of list*/

	fprintf(output_stream,"Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
		head->name,head->start_time, head->duration_minutes);

        head = head->next;
      }
    }
  }
  return SUCCESS;

}

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
  /*malloc size for new_event in the linked list and initialize variables*/
  Event *new_event;
  Event *head = calendar->events[day-1];
  Event *curr = head;
  Event *prev = NULL;

  /*goes to find event in day and checks if it already exits */
  if(find_event(calendar,name, &new_event) == SUCCESS) {
    return FAILURE;
  }

  /*conditions that should equal to failure*/
  if(calendar == NULL || name == NULL || start_time < 0 || start_time > 2400
     || duration_minutes < 0 || day < 1 || calendar->days < day) {
    return FAILURE;
  }

  /*making the new_event*/
  new_event = calloc(ONE,sizeof(Event));
  new_event->name = (char*)calloc(ONE,strlen(name)+ONE);
  strcpy(new_event->name,name);
  new_event->start_time = start_time;
  new_event->duration_minutes = duration_minutes;
  new_event->info = info;
  new_event->next = NULL;

  /*goes to find event in day and checks if it already exits */
  if(find_event(calendar,name, &new_event) == SUCCESS) {
    return FAILURE;
  }

  if(calendar->events[day-1] == NULL) { /*first position of linked list with 0 elements*/
    calendar->events[day-1] = new_event;
    calendar->total_events++;
  }

  else {
    while(curr!=NULL) { /*curr will keep going until it reaches null to add*/

      if(calendar->comp_func(new_event,curr)<0) { /*doing case for middle*/
	if(prev == NULL) { /*when new_event should become the new head*/
	  calendar->events[day-1] = new_event;

	}
	else {
	  prev->next = new_event;
	}
	/*sets the new _event*/
	new_event->next = curr;
	/*adds 1 event to total events*/
	calendar->total_events++;
      	break;

      }
      else if(curr->next == NULL) {/*last case*/
	curr->next = new_event;
	calendar->total_events++;
	break;
      }
      /*curr progresses and prev keeps the curr's position*/
      prev = curr;
      curr = curr->next;

    }
  }

  return SUCCESS;

}

int find_event(Calendar *calendar, const char *name, Event **event) {
  int i; /*this function goes through array of days and checks linked list of events for each day*/
  Event *head, *curr;

  if(calendar == NULL || name == NULL || event == NULL) {
    return FAILURE;
  }

  /*goes through days array and find the event at that day*/
  for(i = 0; i < calendar->days; i++) {

    head = calendar->events[i];
    curr = head;

    while(curr != NULL) {
      /*if the name of curr is same as parameter then it makes shallow copy and outs event*/
      if(strcmp(curr->name,name) == 0) {
	*event = curr;
	return SUCCESS; /*event is found*/
      }

      curr = curr->next;
    }

  }
  return FAILURE; /*not found*/

}

int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
  Event *head = calendar->events[day-1], *curr= head; /*initializes for linked list at that day*/

  if(calendar == NULL || name == NULL || day < 1 || day > calendar->days || event == NULL) {
    return FAILURE;
  }


  while(curr!=NULL) {
    /*checks if currs name is same as name parameter*/
    if(strcmp(curr->name,name) == 0) {
      *event = curr;
      return SUCCESS; /*event is found*/
    }

    curr = curr->next;

  }

  return FAILURE; /*was not found*/
}

int remove_event(Calendar *calendar, const char *name) {
  Event *head, *prev;
  int i;

  if( calendar == NULL || name == NULL) {
    return FAILURE;
  }

  for(i=0; i < calendar->days; i++) { /*linked list is initialized with head and curr in front of it*/
    prev = NULL;
    head = calendar->events[i];

    while(head != NULL) {

      if(strcmp(head->name , name)==0) {

	if(prev == NULL) { /*When new event should become the new head*/
	  calendar->events[i] = calendar->events[i]->next;
	}else {
	  prev->next = prev->next->next;
	}

	if(head->info != NULL && calendar->free_info_func != NULL) { /*if there is info and function is present*/
	  calendar->free_info_func(head->info);
	}

	free(head->name); /*name and the event*/
	free(head);
	head= NULL;

	calendar->total_events--; /*subtracts the events*/

	return SUCCESS;
      }

      prev = head; /*goes to next in list*/
      head = head->next;

    }
  }

  return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
  Event *event;

  if(find_event(calendar,name,&event) == SUCCESS) { /*will find event and then return the info parameter*/
    return event->info;
  }
  return NULL;
}

int clear_calendar(Calendar *calendar) { /*clears all calendar*/
  int i;
  Event *head;


  for(i = 0; i< calendar->days; i++) { /*goes through days*/

    while(calendar->events[i] != NULL) { /*goes through whole linked list of that day if any*/

      head = calendar->events[i]; /*head is set*/

      if(head->info != NULL) { /*frees info*/
	calendar->free_info_func(head->info);
      }

      head->name;

      free(head->name);/*frees name*/


      calendar->events[i] = head->next; /*goes to next of head*/
      calendar->total_events--;
      free(head); /*event itself is freed*/
    }
  }

  return SUCCESS;
}

int clear_day(Calendar *calendar, int day) {/*clears all events for that day*/
  Event *head;

  if(calendar == NULL || day < 1 || day > calendar->days) {
    return FAILURE;
  }


  while(calendar->events[day-1] != NULL) { /*goes through linked list of that day*/
    head = calendar->events[day-1];


    if(head->info!= NULL && calendar->free_info_func!= NULL) { /*if there is info or info_free_func exists*/
      calendar->free_info_func(head->info); /*frees info at the head*/
    }


    free(head->name);
    calendar->events[day-1] = head->next;
    calendar->total_events--; /*subtracts 1 and goes to next event unless is null*/
    free(head);
  }

  return SUCCESS;


}

int destroy_calendar(Calendar *calendar) { /*destroys whole calendar*/
  int i;
  Event *head;

  if(calendar->total_events > 0) {
    for(i = 0; i< calendar->days; i++) { /*goes through days*/

      while(calendar->events[i] != NULL) { /*goes through whole linked list of that day if any*/

	head = calendar->events[i]; /*head is set*/

	if(head->info != NULL) { /*frees info*/
	  calendar->free_info_func(head->info);
	}

	if(head->name != NULL) {

	  free(head->name);/*frees name*/
	}

	calendar->events[i] = head->next; /*goes to next of head*/
	free(head); /*event itself is freed*/
      }
    }
  }


  /*name, event array, and calendar is struct is freed*/
  free(calendar->name);
  free(calendar->events);
  free(calendar);

  return SUCCESS;

}
