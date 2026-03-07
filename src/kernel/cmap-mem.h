#ifndef __CMAP_MEM_H__
#define __CMAP_MEM_H__

#include "cmap-mem-define.h"
#include "cmap-mem-ext.h"

void cmap_mem_delete();

void * cmap_mem_alloc(int size);
void cmap_mem_free(void * ptr);

char cmap_mem_is_internal();

#ifdef CONSUMED_TIME
void cmap_mem_log_consumed_time(char lvl);
#endif

#endif
