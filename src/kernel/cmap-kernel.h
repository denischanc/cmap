#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-kernel-define.h"

void cmap_kernel_exit(int ret);
void cmap_kernel_fatal();

int cmap_kernel_main();

char cmap_kernel_state();

void cmap_kernel_bootstrap(int argc, char ** argv);

#endif
