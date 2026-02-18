#ifndef __CMAP_KCLI_DEFINE_H__
#define __CMAP_KCLI_DEFINE_H__

#define CMAP_KCLI_LOOP(macro) \
  macro(mem.chunk_size, mem_chunk_size) \
  macro(mem.failure_on_alloc, mem_failure_on_alloc) \
  macro(log.lvl, log_lvl) \
  macro(log.path, log_path) \
  macro(refs.check_zombie_time_us, refs_check_zombie_time_us) \
  macro(core.list_chunk_size, core_list_chunk_size) \
  macro(core.string_size_inc_min, core_string_size_inc_min) \
  macro(core.string_size_inc, core_string_size_inc) \
  macro(pool.size, pool_size) \
  macro(modules, modules)

#define CMAP_KCLI_DECL(prop, name) \
  char * cmap_kcli_##name();

#endif
