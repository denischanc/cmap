
#include "cmap-factory.h"

void cmap_object__delete(CMAP_OBJECT * this)
{
  this -> factory_ -> delete(this);
}

void cmap_factory__delete(CMAP_OBJECT * object)
{
  // TODO
}
