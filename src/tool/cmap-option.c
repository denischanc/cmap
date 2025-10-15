
#include "cmap-option.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

/*******************************************************************************
*******************************************************************************/

#define BOOL_VAR(name) static char name = (1 == 0);
#define STRING_VAR(name) static const char * name = NULL;

CMAP_OPTION_LOOP(BOOL_VAR, STRING_VAR)

/*******************************************************************************
*******************************************************************************/

#define BOOL_FN(name) \
static void name##_() \
{ \
  name = (1 == 1); \
} \
 \
static void set_##name(char val) \
{ \
  name = val; \
} \
 \
static char is_##name() \
{ \
  return name; \
}

#define STRING_FN(name) \
static void set_##name(const char * val) \
{ \
  name = val; \
} \
 \
static const char * name##_() \
{ \
  return name; \
}

CMAP_OPTION_LOOP(BOOL_FN, STRING_FN)

/*******************************************************************************
*******************************************************************************/

#define BOOL_SET(name) name##_, set_##name, is_##name,
#define STRING_SET(name) set_##name, name##_,

const CMAP_OPTION_PUBLIC cmap_option_public =
{
  CMAP_OPTION_LOOP(BOOL_SET, STRING_SET)
};
