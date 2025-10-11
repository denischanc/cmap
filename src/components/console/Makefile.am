
include $(top_srcdir)/src/common.mk
include $(top_srcdir)/src/components/common.mk

BUILT_SOURCES = cmap-console-blt.c cmap-console-blt.h

EXTRA_DIST = cmap-console.cmap

noinst_HEADERS = cmap-console.h

noinst_LTLIBRARIES = libconsole.la

nodist_libconsole_la_SOURCES = cmap-console-blt.c
libconsole_la_SOURCES = cmap-console.c
libconsole_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = $(BUILT_SOURCES)
