
include $(top_srcdir)/src/kernel/common.mk

include_HEADERS = \
  cmap-mem.h \
  cmap-map.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-mem.c \
  cmap-map.c
libcore_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
