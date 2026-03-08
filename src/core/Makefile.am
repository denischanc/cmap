
include $(top_srcdir)/src/common.mk

SUBDIRS = cmp op prototype score

include_cmap_HEADERS = \
  cmap-double-ext.h \
  cmap-double-type.h \
  cmap-fn-ext.h \
  cmap-fn-type.h \
  cmap-int-ext.h \
  cmap-int-type.h \
  cmap-list-ext.h \
  cmap-list-type.h \
  cmap-map-ext.h \
  cmap-map-type.h \
  cmap-ptr-ext.h \
  cmap-ptr-type.h \
  cmap-string-ext.h \
  cmap-string-type.h

noinst_HEADERS = \
  cmap-core-define.h \
  cmap-core.h \
  cmap-double-define.h \
  cmap-double.h \
  cmap-double-int.h \
  cmap-fn-define.h \
  cmap-fn.h \
  cmap-initargs.h \
  cmap-int-define.h \
  cmap-int.h \
  cmap-int-int.h \
  cmap-lifecycle-define.h \
  cmap-lifecycle.h \
  cmap-lifecycle-int.h \
  cmap-lifecycle-type.h \
  cmap-list-define.h \
  cmap-list.h \
  cmap-map-define.h \
  cmap-map.h \
  cmap-map-int.h \
  cmap-ptr.h \
  cmap-ptr-int.h \
  cmap-string-define.h \
  cmap-string.h

noinst_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
  cmap-core.c \
  cmap-double.c \
  cmap-fn.c \
  cmap-int.c \
  cmap-lifecycle.c \
  cmap-list.c \
  cmap-map.c \
  cmap-ptr.c \
  cmap-string.c
libcore_la_LIBADD = \
  cmp/libcmp.la \
  op/libop.la \
  prototype/libprototype.la \
  score/libscore.la
