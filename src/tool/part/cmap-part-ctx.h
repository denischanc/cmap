#ifndef __CMAP_PART_CTX_H__
#define __CMAP_PART_CTX_H__

/*******************************************************************************
*******************************************************************************/

#include "cmap-part-ctx-define.h"
#include "cmap-strings.h"
#include "cmap-part-kv.h"
#include "cmap-part-keys.h"

/*******************************************************************************
*******************************************************************************/

typedef struct CMAP_PART_CTX_CMAP CMAP_PART_CTX_CMAP;
typedef struct CMAP_PART_CTX_C CMAP_PART_CTX_C;
typedef struct CMAP_PART_CTX_BLOCK CMAP_PART_CTX_BLOCK;

struct CMAP_PART_CTX_CMAP
{
  CMAP_STRINGS * vars_loc, * vars_def;

  CMAP_PART_CTX_CMAP * prev;
};

struct CMAP_PART_CTX_C
{
  char definitions, global_env, return_, return_fn;

  CMAP_PART_KV * name2map;
  CMAP_PART_KEYS * params;

  CMAP_PART_CTX_CMAP * cmap;

  CMAP_PART_CTX_BLOCK * block;

  CMAP_PART_CTX_C * prev;
};

struct CMAP_PART_CTX_BLOCK
{
  char * instructions, * prefix, else_, nature;

  CMAP_STRINGS * fn_arg_names;

  CMAP_PART_KEYS * affecteds;

  CMAP_PART_CTX_C * c;
};

typedef struct
{
  CMAP_PART_CTX_CMAP cmap;
  CMAP_PART_CTX_C c;
  CMAP_PART_CTX_BLOCK block;
} CMAP_PART_CTX;

/*******************************************************************************
*******************************************************************************/

extern const char CMAP_PART_CTX_NATURE_FN, CMAP_PART_CTX_NATURE_ROOT,
  CMAP_PART_CTX_NATURE_BLOCK, CMAP_PART_CTX_NATURE_PARAMS;

typedef struct
{
  CMAP_PART_CTX_NATURE_LOOP(CMAP_PART_CTX_NATURE_DECL)

  CMAP_PART_CTX (*create)(CMAP_PART_CTX * ctx);
  void (*upd)(CMAP_PART_CTX * ctx);

  void (*delete)(CMAP_PART_CTX * ctx);
} CMAP_PART_CTX_PUBLIC;

extern const CMAP_PART_CTX_PUBLIC cmap_part_ctx_public;

/*******************************************************************************
*******************************************************************************/

#endif
