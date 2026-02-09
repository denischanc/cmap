#ifndef __CMAP_CLI_H__
#define __CMAP_CLI_H__

#include "cmap-cli-define.h"

typedef struct
{
  CMAP_CLI_LOOP(CMAP_CLI_BOOL_DECL, CMAP_CLI_STRING_DECL, CMAP_CLI_STRINGS_DECL)

  void (*mng_opts)(int * argc, char *** argv);

  void (*clean)();
} CMAP_CLI_PUBLIC;

extern const CMAP_CLI_PUBLIC cmap_cli_public;

#endif
