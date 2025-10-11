
include $(top_srcdir)/src/tool/common.mk
include $(top_srcdir)/src/tool/tpl/common.mk

CMAP_PRJ_NATURE = simple

BUILT_SOURCES = \
  cmap-prj-simple-hello-world.cmap.h \
  cmap-prj-simple-Makefile.h

EXTRA_DIST = \
  hello-world.cmap.tpl \
  Makefile.tpl

noinst_LTLIBRARIES = libprj-simple.la

nodist_libprj_simple_la_SOURCES = $(BUILT_SOURCES:.h=.c)
libprj_simple_la_CFLAGS = $(CMAP_TOOL_CFLAGS) $(CMAP_TPL_CFLAGS)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
