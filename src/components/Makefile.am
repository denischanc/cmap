
include $(top_srcdir)/src/common.mk
include $(top_srcdir)/src/components/common.mk

SUBDIRS = console scheduler cli

BUILT_SOURCES = cmap-cmap-blt.c cmap-cmap-blt.h

EXTRA_DIST = cmap-cmap.cmap

noinst_HEADERS = cmap-cmap.h

noinst_LTLIBRARIES = libcomponents.la

nodist_libcomponents_la_SOURCES = cmap-cmap-blt.c
libcomponents_la_SOURCES = cmap-cmap.c
libcomponents_la_CFLAGS = $(CMAP_CFLAGS)
libcomponents_la_LIBADD = \
  console/libconsole.la \
  scheduler/libscheduler.la \
  cli/libcli.la

CLEANFILES = $(BUILT_SOURCES)
