
include $(top_srcdir)/src/common.mk

noinst_HEADERS = cmap-console.h

noinst_LTLIBRARIES = libconsole.la

libconsole_la_SOURCES = cmap-console.c
libconsole_la_CFLAGS = $(CMAP_CFLAGS)

CMAP_CONSOLE_PATH = $(CMAP_SRC_PATH)/components/console
CMAP_CONSOLE_ROOT = $(CMAP_CONSOLE_PATH)/cmap-console
CMAP_CONSOLE_FILES = $(CMAP_CONSOLE_ROOT).c $(CMAP_CONSOLE_ROOT).h

CMAP_MAIN = $(CMAP_BUILD_PATH)/main/cmap-main

$(CMAP_CONSOLE_FILES): $(CMAP_CONSOLE_ROOT).cmap $(CMAP_MAIN)
	CMAP_MAIN=$$PWD/../../main/cmap-main; \
	cd $(CMAP_CONSOLE_PATH); \
	$$CMAP_MAIN gen cmap-console.cmap cmap-console -i
