#ifndef __CMAP_EXEC_H__
#define __CMAP_EXEC_H__

#define CMAP_EXEC_MODULE_NAME "exec"

typedef struct
{
  int (*main)(int argc, char * argv[]);
} CMAP_EXEC_PUBLIC;

extern const CMAP_EXEC_PUBLIC cmap_exec_public;

#endif
