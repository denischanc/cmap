
include $(top_srcdir)/src/common.mk

SUBDIRS = console scheduler cli

EXTRA_DIST = cmap-cmap.cmap

BUILT_SOURCES = cmap-cmap.h

nodist_noinst_HEADERS = cmap-cmap.h
noinst_HEADERS = cmap-cmap-ep.h

noinst_LTLIBRARIES = libcomponents.la

nodist_libcomponents_la_SOURCES = cmap-cmap.c
libcomponents_la_SOURCES = cmap-cmap-ep.c
libcomponents_la_CFLAGS = $(CMAP_CFLAGS)
libcomponents_la_LIBADD = \
  console/libconsole.la \
  scheduler/libscheduler.la \
  cli/libcli.la

CLEANFILES = cmap-cmap.c cmap-cmap.h

CMAP_CMAP_FILE = $(CMAP_SRC_PATH)/components/cmap-cmap.cmap

cmap-cmap.c cmap-cmap.h: $(CMAP_CMAP_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) gen $(CMAP_CMAP_FILE) cmap-cmap -i -fcmap_cmap_public_create
