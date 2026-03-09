
include $(top_srcdir)/src/tool/tpl/common.mk

BUILT_SOURCES = \
  cmap-prj-module-Makefile.h \
  cmap-prj-module-watch-mem.cmap.h \
  cmap-prj-module-watch-mem-fn.c.h \
  cmap-prj-module-watch-mem-fn.h.h

EXTRA_DIST = \
  Makefile.tpl \
  watch-mem.cmap.tpl \
  watch-mem-fn.c.tpl \
  watch-mem-fn.h.tpl

noinst_LTLIBRARIES = libprj-module.la

nodist_libprj_module_la_SOURCES = $(BUILT_SOURCES:.h=.c)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
