#ifndef __CMAP_PARAMS_H__
#define __CMAP_PARAMS_H__

typedef struct
{
  char * decl;
  char * impl;
} PARAMS_RET;

typedef struct
{
  void (*clone)();
  PARAMS_RET (*get)();
  void (*delete)(PARAMS_RET ret);
} CMAP_PARAMS_PUBLIC;

extern const CMAP_PARAMS_PUBLIC cmap_params_public;

#endif
