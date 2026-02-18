#ifndef __CMAP_LOOP_H__
#define __CMAP_LOOP_H__

#include "cmap-loop-type.h"
#include "cmap-loop-int.h"
#include <stdint.h>

struct CMAP_LOOP_EVENT
{
  CMAP_LOOP_EVENT_INTERNAL internal;

  char (*fired)(CMAP_LOOP_EVENT * this, uint64_t time_us);
  void (*run)(CMAP_LOOP_EVENT * this);
};

void cmap_loop_add(CMAP_LOOP_EVENT * event);
void cmap_loop_rm(CMAP_LOOP_EVENT * event);

void cmap_loop_run();

#endif
