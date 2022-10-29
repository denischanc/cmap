
include $(top_srcdir)/src/common.mk

SUBDIRS = prototype console

noinst_HEADERS = \
  cmap-kernel.h \
  cmap-kernel-define.h \
  cmap-global-env.h \
  cmap-aislestore.h \
  cmap-pool.h \
  cmap-pool-handlers.h \
  cmap-aisle.h \
  cmap-log.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-kernel.c \
  cmap-global-env.c \
  cmap-aislestore.c \
  cmap-pool.c \
  cmap-pool-handlers.c \
  cmap-aisle.c \
  cmap-log.c
libkernel_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libkernel_la_LIBADD = \
  prototype/libprototype.la \
  console/libconsole.la
