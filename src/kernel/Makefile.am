
include $(top_srcdir)/src/common.mk

SUBDIRS = core console

include_HEADERS = \
  cmap-kernel.h \
  cmap-kernel-prototype.h \
  cmap-warehouse.h

noinst_HEADERS = \
  cmap-global-env.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-kernel.c \
  cmap-kernel-prototype.c \
  cmap-global-env.c \
  cmap-warehouse.c
libkernel_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libkernel_la_LIBADD = \
  core/libcore.la \
  console/libconsole.la
