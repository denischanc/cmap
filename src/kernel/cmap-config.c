
#include "cmap-config.h"

#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "cmap.h"
#include "cmap-log.h"
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

#define LOOP_MEM(macro_ptr, macro) \
  macro_ptr(mem.this) \
  macro(mem.chunk_size, MEM_CHUNK_SIZE, CMAP_MEM_CHUNK_SIZE_DFT, to_int_) \
  macro(mem.failure_on_alloc, MEM_FAILURE_ON_ALLOC, CMAP_T, to_char)

#define LOOP_LOG(macro_ptr, macro) \
  macro_ptr(log.this) \
  macro(log.lvl, LOG_LVL, CMAP_LOG_FATAL, to_log_lvl) \
  macro(log.path, LOG_PATH, NULL, to_string)

#define LOOP_REFS(macro_ptr, macro) \
  macro(refs.check_zombie_time_us, REFS_CHECK_ZOMBIE_TIME_US, 3000000, \
    to_uint64_t_)

#define LOOP_CORE(macro_ptr, macro) \
  macro(core.list_chunk_size, CORE_LIST_CHUNK_SIZE, 1 << 8, to_int_) \
  macro(core.string_size_inc_min, CORE_STRING_SIZE_INC_MIN, 1 << 6, to_int_) \
  macro(core.string_size_inc, CORE_STRING_SIZE_INC, 1 << 10, to_int_)

#define LOOP_POOL(macro_ptr, macro) \
  macro(pool.size, POOL_SIZE, 1 << 10, to_int_)

#define LOOP(macro_ptr, macro) \
  LOOP_MEM(macro_ptr, macro) \
  LOOP_LOG(macro_ptr, macro) \
  LOOP_REFS(macro_ptr, macro) \
  LOOP_CORE(macro_ptr, macro) \
  LOOP_POOL(macro_ptr, macro)

#define ARG_PREFIX "-Dcmap."
#define ARG_PREFIX_SIZE 7
#define ARG_SIZE_MAX 128

/*******************************************************************************
*******************************************************************************/

static CMAP_CONFIG config;

static CMAP_CONFIG * config_ptr = NULL;

static int argc_ = 0;
static char ** argv_ = NULL;

/*******************************************************************************
*******************************************************************************/

#define TO_NB(type) \
static void to_##type##_(type * cfg, const char * v, type dft, char use_dft) \
{ \
  long i = (v != NULL) ? atol(v) : 0; \
  if(i > 0) *cfg = i; \
  else if(use_dft) *cfg = dft; \
}

TO_NB(int)
TO_NB(uint64_t)

/*******************************************************************************
*******************************************************************************/

static void to_string(const char ** cfg, const char * v, const char * dft,
  char use_dft)
{
  if(v != NULL) *cfg = v;
  else if(use_dft) *cfg = dft;
}

/*******************************************************************************
*******************************************************************************/

static void to_char(char * cfg, const char * v, char dft, char use_dft)
{
  if(v != NULL)
  {
    if(!strcmp(v, "0") || !strcasecmp(v, "false")) { *cfg = CMAP_F; return; }
    if(!strcmp(v, "1") || !strcasecmp(v, "true")) { *cfg = CMAP_T; return; }
  }
  if(use_dft) *cfg = dft;
}

/*******************************************************************************
*******************************************************************************/

#define LOG_IMPL(LVL, lvl, i) if(!strcasecmp(v, #lvl)) { *cfg = i; return; }

static void to_log_lvl(char * cfg, const char * v, char dft, char use_dft)
{
  if(v != NULL)
  {
    CMAP_LOG_LOOP(LOG_IMPL)
  }
  if(use_dft) *cfg = dft;
}

/*******************************************************************************
*******************************************************************************/

#define INSTANCE_IMPL_PTR(var) config.var = NULL;

#define INSTANCE_IMPL(var, ENV, dft, fn) \
  fn(&config.var, getenv("CMAP_" #ENV), dft, (1 == 1));

static CMAP_CONFIG * instance()
{
  if(config_ptr == NULL)
  {
    LOOP(INSTANCE_IMPL_PTR, INSTANCE_IMPL)

    config_ptr = &config;
  }
  return config_ptr;
}

/*******************************************************************************
*******************************************************************************/

static void shift_args(int * i, int * argc, char ** argv)
{
  for(int j = *i; j < *argc - 1; j++) argv[j] = argv[j + 1];
  (*argc)--;
  (*i)--;
}

/*******************************************************************************
*******************************************************************************/

#define INIT_IMPL_PTR(var)

#define INIT_IMPL(var, ENV, dft, fn) \
  if(!strcmp(#var, buffer)) \
  { \
    fn(&config -> var, off + 1, dft, (1 == 0)); \
    shift_args(&i, &argc, argv); \
  }

static void init(int argc, char ** argv)
{
  CMAP_CONFIG * config = instance();

  for(int i = 0; i < argc; i++)
  {
    const char * arg = argv[i];
    if(!strncmp(arg, ARG_PREFIX, ARG_PREFIX_SIZE))
    {
      char * off = strchr(arg, '=');
      if(off != NULL)
      {
        int var_size = (off - arg) - ARG_PREFIX_SIZE;
        if(var_size <= ARG_SIZE_MAX)
        {
          static char buffer[ARG_SIZE_MAX + 1];
          memcpy(buffer, arg + ARG_PREFIX_SIZE, var_size * sizeof(char));
          buffer[var_size] = 0;

          LOOP(INIT_IMPL_PTR, INIT_IMPL)
        }
      }
    }
  }

  argc_ = argc;
  argv_ = argv;
}

/*******************************************************************************
*******************************************************************************/

static int get_argc()
{
  return argc_;
}

static char ** get_argv()
{
  return argv_;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONFIG_PUBLIC cmap_config_public =
  {instance, init, get_argc, get_argv};
