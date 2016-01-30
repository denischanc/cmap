#ifndef __CMAP_FACTORY_H__
#define __CMAP_FACTORY_H__

typedef struct CMAP_OBJECT_s CMAP_OBJECT;
typedef struct CMAP_FACTORY_s CMAP_FACTORY;

struct CMAP_OBJECT_s
{
  CMAP_FACTORY * factory_;

  const char * (*nature)(CMAP_OBJECT * this);

  void (*delete)(CMAP_OBJECT * this);
};

struct CMAP_FACTORY_s
{
  void (*delete)(CMAP_OBJECT * object);
};

void cmap_object__delete(CMAP_OBJECT * this);
void cmap_factory__delete(CMAP_OBJECT * object);

#endif
