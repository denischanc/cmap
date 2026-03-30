
#include "cmap-core.h"

#include "cmap-log.h"
#include "cmap-double.h"
#include "cmap-fn.h"
#include "cmap-int.h"
#include "cmap-list.h"
#include "cmap-map.h"
#include "cmap-ptr.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

#define NATURE_VAR(NAME, name, id, nested, delete) \
  const unsigned char CMAP_##NAME##_NATURE = id;

CMAP_CORE_LOOP(NATURE_VAR)

#define NATURE_CHAR_VAR(NAME, name, id, nested, delete) \
  const char * CMAP_##NAME##_NATURE_CHAR = #name;

CMAP_CORE_LOOP(NATURE_CHAR_VAR)

#define NATURE_ROW(NAME, name, id, nested, delete) #name,

static const char * NATURES[] = {CMAP_CORE_LOOP(NATURE_ROW)};

/*******************************************************************************
*******************************************************************************/

typedef void (*NESTED)(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx);

#define NESTED_ROW(NAME, name, id, nested, delete) cmap_##nested##_nested,

static const NESTED NESTEDS[] = {CMAP_CORE_LOOP(NESTED_ROW)};

void cmap_core_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx)
{
  NESTEDS[lc -> internal.nature](lc, list, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

typedef void (*DELETE)(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);

#define DELETE_ROW(NAME, name, id, nested, delete) cmap_##delete##_delete,

static const DELETE DELETES[] = {CMAP_CORE_LOOP(DELETE_ROW)};

void cmap_core_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  DELETES[lc -> internal.nature](lc, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_core_nature_char(CMAP_LIFECYCLE * lc)
{
  return NATURES[lc -> internal.nature];
}

/*******************************************************************************
*******************************************************************************/

#define LOG_SIZEOF(NAME, name, id, nested, delete) \
  cmap_log(lvl, "sizeof 'CMAP_%s' : '%ld'", #NAME, sizeof(CMAP_##NAME));

void cmap_core_log_sizeof(char lvl)
{
  CMAP_CORE_LOOP(LOG_SIZEOF)
}
