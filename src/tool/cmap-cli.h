#ifndef __CMAP_CLI_H__
#define __CMAP_CLI_H__

#include "cmap-cli-define.h"

CMAP_CLI_LOOP(CMAP_CLI_BOOL_DECL, CMAP_CLI_STRING_DECL, CMAP_CLI_STRINGS_DECL)

void cmap_cli_mng_opts(int * argc, char *** argv);

void cmap_cli_clean();

#endif
