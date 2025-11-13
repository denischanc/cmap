
include $(top_srcdir)/src/tool/tpl/common.mk

BUILT_SOURCES = \
  cmap-prj-simple-hello-world.cmap.h \
  cmap-prj-simple-Makefile.h

EXTRA_DIST = \
  hello-world.cmap.tpl \
  Makefile.tpl

noinst_LTLIBRARIES = libprj-simple.la

nodist_libprj_simple_la_SOURCES = $(BUILT_SOURCES:.h=.c)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
