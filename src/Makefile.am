
include $(top_srcdir)/src/common.mk

SUBDIRS = tool components core kernel loop util

include_cmap_HEADERS = cmap-define-ext.h cmap-ext.h

noinst_HEADERS = cmap-define.h cmap.h

lib_LTLIBRARIES = libcmap.la

libcmap_la_SOURCES = cmap.c
libcmap_la_LDFLAGS = -version-info 1:0:0
libcmap_la_LIBADD = \
  components/libcomponents.la \
  core/libcore.la \
  kernel/libkernel.la \
  loop/libloop.la \
  util/libutil.la

if CYGWIN
libcmap_la_LDFLAGS += -no-undefined
endif
