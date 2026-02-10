
#include "cmap-kcli.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

#define ARG_PREFIX "-Dcmap."
#define ARG_PREFIX_SIZE 7
#define ARG_SIZE_MAX 128

/*******************************************************************************
*******************************************************************************/

#define VAR(prop, name) \
  static const char * name##_val = NULL;

CMAP_KCLI_LOOP(VAR)

static int argc_ = 0;
static char ** argv_ = NULL;

/*******************************************************************************
*******************************************************************************/

#define IMPL_GET(prop, name) \
static const char * name() \
{ \
  return name##_val; \
}

CMAP_KCLI_LOOP(IMPL_GET)

/*******************************************************************************
*******************************************************************************/

static void switch_args(int * i, int * argc, char *** argv)
{
  char ** argv__ = *argv;
  char * cur = argv__[*i];
  for(int j = *i; j > 0; j--) argv__[j] = argv__[j - 1];
  argv__[0] = cur;

  (*i)--;
  (*argc)--;
  (*argv)++;
}

/*******************************************************************************
*******************************************************************************/

#define IMPL_SET(prop, name) \
  if(!strcmp(#prop, buffer)) \
  { \
    name##_val = off + 1; \
    switch_args(&i, &argc, &argv); \
  }

static void parse(int argc, char ** argv)
{
  for(int i = 0; i < argc; i++)
  {
    const char * arg = argv[i];
    if(!strncmp(arg, ARG_PREFIX, ARG_PREFIX_SIZE))
    {
      char * off = strchr(arg, '=');
      if(off != NULL)
      {
        int var_size = (off - arg) - ARG_PREFIX_SIZE;
        if(var_size <= ARG_SIZE_MAX)
        {
          static char buffer[ARG_SIZE_MAX + 1];
          memcpy(buffer, arg + ARG_PREFIX_SIZE, var_size * sizeof(char));
          buffer[var_size] = 0;

          CMAP_KCLI_LOOP(IMPL_SET)
        }
      }
    }
  }

  argc_ = argc;
  argv_ = argv;
}

/*******************************************************************************
*******************************************************************************/

static int get_argc()
{
  return argc_;
}

static char ** get_argv()
{
  return argv_;
}

/*******************************************************************************
*******************************************************************************/

#define SET(prop, name) name,

const CMAP_KCLI_PUBLIC cmap_kcli_public =
{
  CMAP_KCLI_LOOP(SET)
  parse, get_argc, get_argv
};
