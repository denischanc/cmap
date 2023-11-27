
include $(top_srcdir)/src/tool/common.mk
include $(top_srcdir)/src/tool/tpl/common.mk

EXTRA_DIST = \
  hello-world.cmap.tpl \
  Makefile.tpl

nodist_noinst_HEADERS = \
  cmap-prj-simple-hello-world-cmap.h \
  cmap-prj-simple-makefile.h

noinst_LTLIBRARIES = libprj-simple.la

nodist_libprj_simple_la_SOURCES = \
  cmap-prj-simple-hello-world-cmap.c \
  cmap-prj-simple-makefile.c
libprj_simple_la_CFLAGS = $(CMAP_TOOL_CFLAGS) $(CMAP_TPL_CFLAGS)

CLEANFILES = \
  $(nodist_noinst_HEADERS) \
  $(nodist_libprj_simple_la_SOURCES)

HELLO_WORLD_CMAP_BLT = \
  cmap-prj-simple-hello-world-cmap.h \
  cmap-prj-simple-hello-world-cmap.c
$(HELLO_WORLD_CMAP_BLT): hello-world.cmap.tpl

MAKEFILE_BLT = cmap-prj-simple-makefile.h cmap-prj-simple-makefile.c
$(MAKEFILE_BLT): Makefile.tpl
