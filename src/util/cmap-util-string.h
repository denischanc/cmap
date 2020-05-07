#ifndef __CMAP_UTIL_STRING_H__
#define __CMAP_UTIL_STRING_H__

#include "cmap-list.h"

CMAP_LIST * cmap_split_w_aisle(const char * line, char sep, const char * aisle);
CMAP_LIST * cmap_split_w_pool(const char * line, char sep);

#endif
