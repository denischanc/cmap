
include $(top_srcdir)/src/common.mk

SUBDIRS = prototype

noinst_HEADERS = \
  cmap-map.h \
  cmap-map-define.h \
  cmap-list.h \
  cmap-list-define.h \
  cmap-fn.h \
  cmap-string.h \
  cmap-int.h \
  cmap-double.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-map.c \
  cmap-list.c \
  cmap-fn.c \
  cmap-string.c \
  cmap-int.c \
  cmap-double.c
libcore_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libcore_la_LIBADD = prototype/libprototype.la
