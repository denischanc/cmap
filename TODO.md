TODO
====

## Kernel

* refactor pool, map ?
* prototype
** creation KO (map not initialized)
** map: rm new(), add it in init
** it must be referenced
* cmap-tuning.h: memory, chunk size; pool, size; slist, chunk size; ...
* map: (key, val) in pool
* is_log_lvl_enabled()
* util, util_map, util_list, util_mem
* realloc, calloc for uv
* add add_if_not_eq in stree and modify sset add
* add consumed_time in refswatcher
* cmap-uv.c

## Parser

* remove CMAP block from parser
* mng abc.cde.efg in parser with name2map

## Doc

* stree
