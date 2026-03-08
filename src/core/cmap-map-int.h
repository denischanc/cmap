#ifndef __CMAP_MAP_INT_H__
#define __CMAP_MAP_INT_H__

typedef struct CMAP_MAP_ENTRY CMAP_MAP_ENTRY;

typedef struct
{
  CMAP_MAP_ENTRY * entry_stree;

  CMAP_MAP * prototype;

  char ghost;
} CMAP_MAP_INTERNAL;

#endif
