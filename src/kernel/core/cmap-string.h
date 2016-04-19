#ifndef __CMAP_STRING_H__
#define __CMAP_STRING_H__

#include "cmap-core.h"
#include "cmap-map.h"

extern const char * CMAP_STRING_NATURE;

struct CMAP_STRING_s
{
  CMAP_MAP super_;

  void * internal_;

  const char * (*val)(CMAP_STRING * this);

  void (*append)(CMAP_STRING * this, const char * val);
};

void cmap_string__append(CMAP_STRING * this, const char * val);

CMAP_STRING * cmap_string_create(const char * val);
void cmap_string_init(CMAP_STRING * string, const char * val);
void cmap_string_delete(CMAP_STRING * string);

#endif
