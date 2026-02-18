
#include "cmap-loop.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_LOOP_EVENT * events = NULL, * next_run = NULL;

/*******************************************************************************
*******************************************************************************/

void cmap_loop_add(CMAP_LOOP_EVENT * event)
{
  event -> internal.next = events;

  if(events != NULL) events -> internal.prev = event;
  events = event;
}

void cmap_loop_rm(CMAP_LOOP_EVENT * event)
{
  CMAP_LOOP_EVENT * next = event -> internal.next;

  if(event == next_run) next_run = next;

  if(event == events) events = next;
  else
  {
    CMAP_LOOP_EVENT * prev = event -> internal.prev;
    prev -> internal.next = next;
    if(next != NULL) next -> internal.prev = prev;
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_loop_run()
{
  while(events != NULL)
  {
    uint64_t time_us = cmap_util_time_us();
    CMAP_LOOP_EVENT * cur = events;
    while(cur != NULL)
    {
      next_run = cur -> internal.next;
      if(CMAP_CALL_ARGS(cur, fired, time_us)) CMAP_CALL(cur, run);
      cur = next_run;
    }
  }
}
