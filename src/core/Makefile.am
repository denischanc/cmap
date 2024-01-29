
include $(top_srcdir)/src/common.mk

SUBDIRS = cmp prototype score

include_cmap_HEADERS = \
  cmap-map-type.h \
  cmap-map-ext.h \
  cmap-list-type.h \
  cmap-list-ext.h \
  cmap-fn-type.h \
  cmap-fn-ext.h \
  cmap-string-type.h \
  cmap-string-ext.h \
  cmap-int-type.h \
  cmap-int-ext.h \
  cmap-double-type.h \
  cmap-double-ext.h \
  cmap-ptr-type.h \
  cmap-ptr-ext.h

noinst_HEADERS = \
  cmap-initargs.h \
  cmap-map.h \
  cmap-map-define.h \
  cmap-list.h \
  cmap-list-define.h \
  cmap-fn.h \
  cmap-fn-define.h \
  cmap-string.h \
  cmap-string-define.h \
  cmap-int.h \
  cmap-int-define.h \
  cmap-double.h \
  cmap-double-define.h \
  cmap-ptr.h \
  cmap-lifecycle.h \
  cmap-lifecycle-define.h \
  cmap-lifecycle-type.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-map.c \
  cmap-list.c \
  cmap-fn.c \
  cmap-string.c \
  cmap-int.c \
  cmap-double.c \
  cmap-ptr.c \
  cmap-lifecycle.c
libcore_la_CFLAGS = $(CMAP_CFLAGS)
libcore_la_LIBADD = cmp/libcmp.la prototype/libprototype.la score/libscore.la
