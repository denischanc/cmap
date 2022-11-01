
include $(top_srcdir)/src/common.mk

noinst_HEADERS = \
  cmap-mem.h \
  cmap-mem-define.h \
  cmap-kernel.h \
  cmap-kernel-define.h \
  cmap-global-env.h \
  cmap-aislestore.h \
  cmap-pool.h \
  cmap-pool-handlers.h \
  cmap-aisle.h \
  cmap-log.h \
  cmap-log-define.h

noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-mem.c \
  cmap-kernel.c \
  cmap-global-env.c \
  cmap-aislestore.c \
  cmap-pool.c \
  cmap-pool-handlers.c \
  cmap-aisle.c \
  cmap-log.c
libkernel_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
