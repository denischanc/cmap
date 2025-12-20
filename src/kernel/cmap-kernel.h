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

typedef struct
{
  CMAP_KERNEL * (*instance)();

  void (*bootstrap)(int argc, char ** argv);
} CMAP_KERNEL_PUBLIC;

extern const CMAP_KERNEL_PUBLIC cmap_kernel_public;

#endif
