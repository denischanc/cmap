
include $(top_srcdir)/src/common.mk

EXTRA_DIST = cmap-console.cmap

nodist_noinst_HEADERS = cmap-console-blt.h
noinst_HEADERS = cmap-console.h

noinst_LTLIBRARIES = libconsole.la

nodist_libconsole_la_SOURCES = cmap-console-blt.c
libconsole_la_SOURCES = cmap-console.c
libconsole_la_CFLAGS = $(CMAP_CFLAGS)

CLEANFILES = cmap-console-blt.c cmap-console-blt.h

CMAP_CONSOLE_FILE = $(CMAP_SRC_PATH)/components/console/cmap-console.cmap

cmap-console-blt.c cmap-console-blt.h: $(CMAP_CONSOLE_FILE) $(CMAP_TOOL)
	$(CMAP_TOOL) build $(CMAP_CONSOLE_FILE) cmap-console-blt \
	  -i -fcmap_console_blt_public_create
