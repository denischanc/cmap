#ifndef __CMAP_ROOT_H__
#define __CMAP_ROOT_H__

typedef struct CMAP_ROOT_s CMAP_ROOT;

struct CMAP_ROOT_s
{
  const char * (*nature)(CMAP_ROOT * this);

  void (*delete)(CMAP_ROOT * this);
};

void cmap_root_init(CMAP_ROOT * root);
void cmap_root_delete(CMAP_ROOT * root);

#endif
