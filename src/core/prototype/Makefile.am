
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-prototype-map.h \
  cmap-prototype-list.h \
  cmap-prototype-fn.h \
  cmap-prototype-string.h \
  cmap-prototype-int.h \
  cmap-prototype-double.h \
  cmap-prototype-ptr.h \
  cmap-prototype-util.h \
  cmap-prototype-util-define.h \
  cmap-prototypestore.h \
  cmap-prototypestore-define.h

noinst_LTLIBRARIES = libprototype.la

libprototype_la_SOURCES = \
  cmap-prototype-map.c \
  cmap-prototype-list.c \
  cmap-prototype-fn.c \
  cmap-prototype-string.c \
  cmap-prototype-int.c \
  cmap-prototype-double.c \
  cmap-prototype-ptr.c \
  cmap-prototype-util.c \
  cmap-prototypestore.c
libprototype_la_CFLAGS = $(CMAP_CFLAGS)
