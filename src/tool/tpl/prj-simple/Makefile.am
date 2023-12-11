
include $(top_srcdir)/src/tool/common.mk
include $(top_srcdir)/src/tool/tpl/common.mk

CMAP_PRJ_NATURE = simple

BUILT_SOURCES = $(nodist_noinst_HEADERS)

EXTRA_DIST = \
  hello-world.cmap.tpl \
  Makefile.tpl

nodist_noinst_HEADERS = \
  cmap-prj-simple-hello-world.cmap.h \
  cmap-prj-simple-Makefile.h

noinst_LTLIBRARIES = libprj-simple.la

nodist_libprj_simple_la_SOURCES = $(nodist_noinst_HEADERS:.h=.c)
libprj_simple_la_CFLAGS = $(CMAP_TOOL_CFLAGS) $(CMAP_TPL_CFLAGS)

CLEANFILES = \
  $(nodist_noinst_HEADERS) \
  $(nodist_libprj_simple_la_SOURCES)
