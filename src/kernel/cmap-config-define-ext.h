#ifndef __CMAP_CONFIG_DEFINE_EXT_H__
#define __CMAP_CONFIG_DEFINE_EXT_H__

#include <stdint.h>
#include "cmap-mem-type.h"
#include "cmap-log-type.h"

#define CMAP_CONFIG_LOOP(macro) \
  macro(mem_chunk_size, int, to_int_, MEM_CHUNK_SIZE, CMAP_MEM_CHUNK_SIZE_DFT) \
  macro(mem_failure_on_alloc, char, to_char, MEM_FAILURE_ON_ALLOC, CMAP_T) \
  macro(log_lvl, char, to_log_lvl, LOG_LVL, CMAP_LOG_FATAL) \
  macro(log_path, const char *, to_string, LOG_PATH, NULL) \
  macro(refs_check_zombie_time_us, uint64_t, to_uint64_t_, \
    REFS_CHECK_ZOMBIE_TIME_US, 3000000) \
  macro(core_list_chunk_size, int, to_int_, CORE_LIST_CHUNK_SIZE, 1 << 8) \
  macro(core_string_size_inc_min, int, to_int_, CORE_STRING_SIZE_INC_MIN, \
    1 << 6) \
  macro(core_string_size_inc, int, to_int_, CORE_STRING_SIZE_INC, 1 << 10) \
  macro(pool_size, int, to_int_, POOL_SIZE, 1 << 10)

#define CMAP_CONFIG_DECL(name, type, fn, ENV_VAR, dft) \
  void cmap_config_set_##name(type v); \
  type cmap_config_##name();

#define CMAP_CONFIG_LOOP_PTR(macro) \
  macro(mem, CMAP_MEM) \
  macro(log, CMAP_LOG)

#define CMAP_CONFIG_DECL_PTR(name, type) \
  void cmap_config_set_##name(type * name); \
  type * cmap_config_##name();

#endif
