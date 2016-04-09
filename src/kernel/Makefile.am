
include $(top_srcdir)/src/common.mk

SUBDIRS = core

include_HEADERS = \
  cmap-kernel.h \
  cmap-kernel-prototype.h
noinst_LTLIBRARIES = libkernel.la

libkernel_la_SOURCES = \
  cmap-kernel.c \
  cmap-kernel-prototype.c
libkernel_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
libkernel_la_LIBADD = core/libcore.la
