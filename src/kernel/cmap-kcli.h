#ifndef __CMAP_KCLI_H__
#define __CMAP_KCLI_H__

#include "cmap-kcli-define.h"

CMAP_KCLI_LOOP(CMAP_KCLI_DECL)

void cmap_kcli_parse(int argc, char ** argv);
int cmap_kcli_argc();
char ** cmap_kcli_argv();

#endif
