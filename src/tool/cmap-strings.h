#ifndef __CMAP_STRINGS_H__
#define __CMAP_STRINGS_H__

#include "cmap-stack-define.h"

CMAP_STACK_TYPE(STRINGS)
typedef CMAP_STACK_STRINGS CMAP_STRINGS;

typedef void (*CMAP_STRINGS_STRING_FN)(const char * string, void * data);

typedef struct
{
  int (*contains)(CMAP_STRINGS * strings, const char * string);

  void (*add)(CMAP_STRINGS ** strings_ptr, const char * string);

  void (*apply)(CMAP_STRINGS * strings, CMAP_STRINGS_STRING_FN fn, void * data);

  void (*add_all)(CMAP_STRINGS ** strings_ptr, CMAP_STRINGS * others);

  void (*delete)(CMAP_STRINGS ** strings_ptr);
} CMAP_STRINGS_PUBLIC;

extern const CMAP_STRINGS_PUBLIC cmap_strings_public;

#endif
