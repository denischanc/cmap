#ifndef __CMAP_STRING_H__
#define __CMAP_STRING_H__

#include "cmap-string-define.h"
#include "cmap-string-ext.h"
#include "cmap-string-int.h"
#include "cmap-map.h"

struct CMAP_STRING
{
  CMAP_MAP super;

  CMAP_STRING_INTERNAL internal;
};

void cmap_string_append(CMAP_STRING * string, const char * val);
void cmap_string_append_sub(CMAP_STRING * string, const char * val,
  int off_start, int off_stop);

void cmap_string_clean(CMAP_STRING * string);

void cmap_string_delete(CMAP_LIFECYCLE * lc);
CMAP_STRING * cmap_string_init(CMAP_STRING * string, CMAP_INITARGS * initargs,
  const char * val, int size_inc);
CMAP_STRING * cmap_string_create(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx);

#endif
