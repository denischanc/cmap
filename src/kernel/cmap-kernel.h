#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#define CMAP_KERNEL_S_ALIVE 0
#define CMAP_KERNEL_S_EXITING 1

typedef struct
{
  int (*main)();

  void (*exit)(int ret);
  void (*fatal)();

  char (*state)();
} CMAP_KERNEL;

CMAP_KERNEL * cmap_kernel_instance();

void cmap_kernel_bootstrap(int argc, char ** argv);

#endif
