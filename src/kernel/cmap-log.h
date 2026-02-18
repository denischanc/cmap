#ifndef __CMAP_LOG_H__
#define __CMAP_LOG_H__

#include "cmap-log-ext.h"
#include "cmap-log-define.h"

void cmap_log_delete();

void cmap_log(char lvl, const char * msg, ...);
void cmap_vlog(char lvl, const char * msg, va_list args);

CMAP_LOG_LOOP(CMAP_LOG_DECL)

#endif
