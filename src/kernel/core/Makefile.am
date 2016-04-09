
include $(top_srcdir)/src/common.mk

include_HEADERS = \
  cmap-mem.h \
  cmap-map.h \
  cmap-list.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-mem.c \
  cmap-map.c \
  cmap-list.c
libcore_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
