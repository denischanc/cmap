
#include "cmap-cli.h"

#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

#define BOOL_VAR(ID, name, long_opt, opt, desc) \
  static char name##_val = (1 == 0);

#define STRING_VAR(ID, name, long_opt, opt, desc) \
  static char * name##_val = NULL;

#define STRINGS_VAR(ID, name, long_opt, opt, desc) \
  static CMAP_STRINGS * name##_val = NULL;

CMAP_CLI_LOOP(BOOL_VAR, STRING_VAR, STRINGS_VAR)

/*******************************************************************************
*******************************************************************************/

#define BOOL_IMPL(ID, name, long_opt, opt, desc) \
char cmap_cli_is_##name() \
{ \
  return name##_val; \
}

#define STRING_IMPL(ID, name, long_opt, opt, desc) \
const char * cmap_cli_##name() \
{ \
  return name##_val; \
}

#define STRINGS_IMPL(ID, name, long_opt, opt, desc) \
CMAP_STRINGS * cmap_cli_##name() \
{ \
  return name##_val; \
}

CMAP_CLI_LOOP(BOOL_IMPL, STRING_IMPL, STRINGS_IMPL)

/*******************************************************************************
*******************************************************************************/

#define ALL_NB(ID, name, long_opt, opt, desc) nb++;

static inline int nb_opts()
{
  int nb = 0;
  CMAP_CLI_LOOP(ALL_NB, ALL_NB, ALL_NB)
  return nb;
}

/*******************************************************************************
*******************************************************************************/

#define NO_ARG_OPTS(ID, name, long_opt, opt, desc) \
  struct option opt_##name = {#long_opt, no_argument, NULL, opt}; \
  long_opts[i++] = opt_##name; \
  cmap_string_append_args(&short_opts, "%c", opt);

#define ARG_OPTS(ID, name, long_opt, opt, desc) \
  struct option opt_##name = {#long_opt, required_argument, NULL, opt}; \
  long_opts[i++] = opt_##name; \
  cmap_string_append_args(&short_opts, "%c:", opt);

#define BOOL_CASE(ID, name, long_opt, opt, desc) \
  case opt: name##_val = (1 == 1); break;

#define STRING_CASE(ID, name, long_opt, opt, desc) \
  case opt: free(name##_val); name##_val = strdup(optarg); break;

#define STRINGS_CASE(ID, name, long_opt, opt, desc) \
  case opt: cmap_strings_add(&name##_val, optarg); break;

void cmap_cli_mng_opts(int * argc, char *** argv)
{
  int argc_bup = *argc;
  for(int i = 0; (i < *argc) && (*argc == argc_bup); i++)
    if(!strcmp((*argv)[i], "--")) *argc = i;

  struct option last_opt = {NULL, 0, NULL, 0},
    * long_opts = malloc((nb_opts() + 1) * sizeof(struct option));
  char * short_opts = NULL;

  int i = 0;
  CMAP_CLI_LOOP(NO_ARG_OPTS, ARG_OPTS, ARG_OPTS)
  long_opts[i] = last_opt;

  optind = 1;
  int o;
  while((o = getopt_long(*argc, *argv, short_opts, long_opts, NULL)) != -1)
  {
    switch(o)
    {
      CMAP_CLI_LOOP(BOOL_CASE, STRING_CASE, STRINGS_CASE)
      default: help_val = (1 == 1);
    }
  }

  free(long_opts);
  free(short_opts);

  *argc = argc_bup - optind;
  *argv += optind;
}

/*******************************************************************************
*******************************************************************************/

#define BOOL_CLEAN(ID, name, long_opt, opt, desc) \
  name##_val = (1 == 0);

#define STRING_CLEAN(ID, name, long_opt, opt, desc) \
  free(name##_val); name##_val = NULL;

#define STRINGS_CLEAN(ID, name, long_opt, opt, desc) \
  cmap_strings_delete(&name##_val);

void cmap_cli_clean()
{
  CMAP_CLI_LOOP(BOOL_CLEAN, STRING_CLEAN, STRINGS_CLEAN)
}
