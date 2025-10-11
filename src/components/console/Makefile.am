
include $(top_srcdir)/src/common.mk
include $(top_srcdir)/src/components/common.mk

EXTRA_DIST = cmap-console.cmap

BUILT_SOURCES = $(EXTRA_DIST:.cmap=-blt.h)

noinst_HEADERS = cmap-console.h

noinst_LTLIBRARIES = libconsole.la

nodist_libconsole_la_SOURCES = $(BUILT_SOURCES:.h=.c)
libconsole_la_SOURCES = cmap-console.c
libconsole_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = $(BUILT_SOURCES) $(BUILT_SOURCES:.h=.c)
