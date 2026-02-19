#ifndef __CMAP_STRINGS_H__
#define __CMAP_STRINGS_H__

#include "cmap-stack-define.h"

CMAP_STACK_TYPE(STRINGS)
typedef CMAP_STACK_STRINGS CMAP_STRINGS;

typedef void (*CMAP_STRINGS_STRING_FN)(const char * string, void * data);

int cmap_strings_contains(CMAP_STRINGS * strings, const char * string);

char cmap_strings_add(CMAP_STRINGS ** strings_ptr, const char * string);

void cmap_strings_set(CMAP_STRINGS * strings, int off, const char * string);

void cmap_strings_apply(CMAP_STRINGS * strings, CMAP_STRINGS_STRING_FN fn,
  void * data);

void cmap_strings_add_all(CMAP_STRINGS ** strings_ptr, CMAP_STRINGS * others);

CMAP_STRINGS * cmap_strings_split(const char * string);

void cmap_strings_delete(CMAP_STRINGS ** strings_ptr);

#endif
