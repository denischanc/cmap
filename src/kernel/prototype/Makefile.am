
include $(top_srcdir)/src/common.mk

include_HEADERS = \
  cmap-prototype.h

noinst_HEADERS = \
  cmap-prototype-map.h \
  cmap-prototype-list.h \
  cmap-prototype-fn.h \
  cmap-prototype-string.h \
  cmap-prototype-int.h \
  cmap-prototype-util.h

noinst_LTLIBRARIES = libprototype.la

libprototype_la_SOURCES = \
  cmap-prototype.c \
  cmap-prototype-map.c \
  cmap-prototype-list.c \
  cmap-prototype-fn.c \
  cmap-prototype-string.c \
  cmap-prototype-int.c \
  cmap-prototype-util.c
libprototype_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
