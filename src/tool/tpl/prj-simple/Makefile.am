
include $(top_srcdir)/src/tool/common.mk
include $(top_srcdir)/src/tool/tpl/common.mk

CMAP_PRJ_NATURE = simple

EXTRA_DIST = \
  hello-world.cmap.tpl \
  Makefile.tpl

BUILT_SOURCES = \
  cmap-prj-simple-hello-world.cmap.h \
  cmap-prj-simple-Makefile.h

nodist_noinst_HEADERS = $(BUILT_SOURCES)

noinst_LTLIBRARIES = libprj-simple.la

nodist_libprj_simple_la_SOURCES = $(BUILT_SOURCES:.h=.c)
libprj_simple_la_CFLAGS = $(CMAP_TOOL_CFLAGS) $(CMAP_TPL_CFLAGS)

CLEANFILES = \
  $(nodist_noinst_HEADERS) \
  $(nodist_libprj_simple_la_SOURCES)
