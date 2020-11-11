#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"


static int comp_minutes(const void *ptr1, const void *ptr2) {
  return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/*This test checks if a duplicate event can be added, then see if event can be added with wrong start time, then it removes the event but it will remove it two times, then it will test whether day was clear then it will clear the day again */
static int test1() {
  int start = 1300, duration = 55, days = 5;
  Calendar *calendar;

  if (init_calendar("First Test Calendar", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if(add_event(calendar,"CMSC216",start, duration, NULL, 3) == SUCCESS) {
      if(add_event(calendar,"CMSC216",start,66, NULL, 1) == FAILURE) { /*double add testing end*/
      	if(add_event(calendar,"CMSC330",1700,80, NULL,2 ) == SUCCESS) {
	  if(add_event(calendar,"CMSC351",1000,70, NULL,4 ) == SUCCESS) {
	    if(add_event(calendar,"test fail",2500,80, NULL,2 ) == FAILURE) { /*wrong start*/
	      if(remove_event(calendar,"CMSC216") == SUCCESS) {
		if(remove_event(calendar,"CMSC216") == FAILURE){ /*should not work because it was already removed*/
		  if(add_event(calendar,"CMSC400",1000,70, NULL,5 ) == SUCCESS){
		    if(add_event(calendar,"new class",1600,70, NULL,5 ) == SUCCESS){
		      if(add_event(calendar,"to be cleared", 500 ,70, NULL, 2 ) == SUCCESS){
		        if(clear_day(calendar, 2) == SUCCESS) {
			  if(add_event(calendar,"Newest Class", 503 ,70, NULL, 5 ) == SUCCESS) {
			    if(clear_day(calendar, 2) == SUCCESS) { /*tries to remove again from already emptyday*/
			      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
				return destroy_calendar(calendar);
			      }
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return FAILURE;
}

/*This test checks if a duplicate event can be added, then see if event can be added with wrong start time, then it removes the event but it will remove it two times,and it also tests clear calendar it also tests if the days is more than calendar */
static int test2() {
  int start = 2100, duration = 60, days = 6;
  Calendar *calendar;

  if (init_calendar("Second Test Calendar", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if(add_event(calendar,"CMSC216",start, duration, NULL, 3) == SUCCESS) {
      if(add_event(calendar,"CMSC216",start,66, NULL, 1) == FAILURE) { /*double add testing end*/
      	if(add_event(calendar,"CMSC330",1700,80, NULL,7 ) == FAILURE) {
	  if(add_event(calendar,"CMSC351",1000,70, NULL,4 ) == SUCCESS) {
	    if(add_event(calendar,"test fail",2500,80, NULL,2 ) == FAILURE) { /*wrong start*/
	      if(remove_event(calendar,"CMSC216") == SUCCESS) {
		if(remove_event(calendar,"CMSC216") == FAILURE){ /*should not work because it was already removed*/
		  if(add_event(calendar,"CMSC400",1000,75, NULL,5 ) == SUCCESS){
		    if(add_event(calendar,"new class",1600,70, NULL,5 ) == SUCCESS){
		      if(add_event(calendar,"to be cleared", 500 ,70, NULL, 2 ) == SUCCESS){
		        if(clear_calendar(calendar) == SUCCESS) { /*clears calendar*/
			  if(add_event(calendar,"Newest Class", 503 ,70, NULL, 5 ) == SUCCESS) {
			    if (print_calendar(calendar, stdout, 1) == SUCCESS) {
			      return destroy_calendar(calendar); /*should just remove the array because no events*/
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  return FAILURE;
}

int main() {
  int result = SUCCESS;


  if (test1() == FAILURE) result = FAILURE;
  if (test2() == FAILURE) result = FAILURE;


  if (result == FAILURE) {
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
