
#include "cmap-config.h"

#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "cmap.h"
#include "cmap-log.h"
#include "cmap-kcli.h"
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

#define VAR(name, type, fn, ENV_VAR, dft) \
  static char name##_val_ok = CMAP_F; \
  static type name##_val; \
  static type name##_dft = dft;

CMAP_CONFIG_LOOP(VAR)

#define VAR_PTR(name, type) \
  static type * name##_val = NULL;

CMAP_CONFIG_LOOP_PTR(VAR_PTR)

/*******************************************************************************
*******************************************************************************/

#define TO_NB(type) \
static type to_##type##_(const char * v, type dft) \
{ \
  if(v == NULL) return dft; \
 \
  long l = atol(v); \
  return (l > 0) ? l : dft; \
}

TO_NB(int)
TO_NB(uint64_t)

/*******************************************************************************
*******************************************************************************/

static char * to_string(char * v, char * dft)
{
  return (v != NULL) ? v : dft;
}

/*******************************************************************************
*******************************************************************************/

static char to_char(const char * v, char dft)
{
  if(v != NULL)
  {
    if(!strcmp(v, "0") || !strcasecmp(v, "false")) return CMAP_F;
    if(!strcmp(v, "1") || !strcasecmp(v, "true")) return CMAP_T;
  }
  return dft;
}

/*******************************************************************************
*******************************************************************************/

#define LOG_IMPL(LVL, lvl) if(!strcasecmp(v, #lvl)) return CMAP_LOG_##LVL;

static char to_log_lvl(const char * v, char dft)
{
  if(v != NULL)
  {
    CMAP_LOG_LOOP(LOG_IMPL)
  }
  return dft;
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, type, fn, ENV_VAR, dft) \
void cmap_config_set_##name(type v) \
{ \
  name##_val = v; \
  name##_val_ok = CMAP_T; \
} \
 \
type cmap_config_##name() \
{ \
  if(!name##_val_ok) \
  { \
    char * tmp = cmap_kcli_##name(); \
    if(tmp == NULL) tmp = getenv("CMAP_" #ENV_VAR); \
    cmap_config_set_##name(fn(tmp, name##_dft)); \
  } \
  return name##_val; \
}

CMAP_CONFIG_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define IMPL_PTR(name, type) \
void cmap_config_set_##name(type * name) \
{ \
  name##_val = name; \
} \
 \
type * cmap_config_##name() \
{ \
  return name##_val; \
}

CMAP_CONFIG_LOOP_PTR(IMPL_PTR)
