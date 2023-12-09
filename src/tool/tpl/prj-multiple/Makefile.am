
include $(top_srcdir)/src/tool/common.mk
include $(top_srcdir)/src/tool/tpl/common.mk

CMAP_PRJ_NATURE = multiple

EXTRA_DIST = \
  main.cmap.tpl \
  Makefile.tpl \
  random.cmap.tpl \
  random-fn.c.tpl \
  random-fn.h.tpl \
  screen.cmap.tpl \
  snake.cmap.tpl

BUILT_SOURCES = \
  cmap-prj-multiple-main.cmap.h \
  cmap-prj-multiple-Makefile.h \
  cmap-prj-multiple-random.cmap.h \
  cmap-prj-multiple-random-fn.c.h \
  cmap-prj-multiple-random-fn.h.h \
  cmap-prj-multiple-screen.cmap.h \
  cmap-prj-multiple-snake.cmap.h

nodist_noinst_HEADERS = $(BUILT_SOURCES)

noinst_LTLIBRARIES = libprj-multiple.la

nodist_libprj_multiple_la_SOURCES = $(BUILT_SOURCES:.h=.c)
libprj_multiple_la_CFLAGS = $(CMAP_TOOL_CFLAGS) $(CMAP_TPL_CFLAGS)

CLEANFILES = \
  $(nodist_noinst_HEADERS) \
  $(nodist_libprj_multiple_la_SOURCES)
