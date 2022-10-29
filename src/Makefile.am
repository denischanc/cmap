
include $(top_srcdir)/src/common.mk

SUBDIRS = util core kernel

noinst_HEADERS = \
  cmap-common-define.h
include_cmap_HEADERS = \
  cmap-define.h \
  cmap.h

lib_LTLIBRARIES = libcmap.la

libcmap_la_SOURCES = cmap.c
libcmap_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libcmap_la_LDFLAGS = -version-info 1:0:0
libcmap_la_LIBADD = \
  util/libutil.la \
  core/libcore.la \
  kernel/libkernel.la

if CYGWIN
libcmap_la_LDFLAGS += -no-undefined
endif
