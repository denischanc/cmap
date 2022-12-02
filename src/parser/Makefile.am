
include $(top_srcdir)/src/common.mk

BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_
### AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-scanner.h \
  cmap-parser-util.h \
  cmap-parser-util-define.h

noinst_LTLIBRARIES = libparser.la

libparser_la_SOURCES = \
  cmap-scanner.l \
  cmap-parser.y \
  cmap-parser-util.c
libparser_la_CFLAGS = $(CMAP_KERNEL_CFLAGS)
