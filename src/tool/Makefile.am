
include $(top_srcdir)/src/tool/common.mk

SUBDIRS = parser-util part tpl

BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-build.h \
  cmap-build-main.h \
  cmap-clean.h \
  cmap-file-util.h \
  cmap-fn-name-define.h \
  cmap-fn-name.h \
  cmap-option-define.h \
  cmap-option.h \
  cmap-parser-util-define.h \
  cmap-parser-util.h \
  cmap-part-define.h \
  cmap-part.h \
  cmap-prj.h \
  cmap-scanner.h \
  cmap-stack-define.h \
  cmap-string.h \
  cmap-strings.h \
  cmap-util.h

bin_PROGRAMS = cmap-tool

cmap_tool_SOURCES = \
  cmap-build.c \
  cmap-build-main.c \
  cmap-clean.c \
  cmap-file-util.c \
  cmap-fn-name.c \
  cmap-option.c \
  cmap-parser-util.c \
  cmap-parser.y \
  cmap-part.c \
  cmap-prj.c \
  cmap-scanner.l \
  cmap-string.c \
  cmap-strings.c \
  cmap-tool.c \
  cmap-util.c
cmap_tool_CFLAGS = $(CMAP_TOOL_CFLAGS)
cmap_tool_LDADD = parser-util/libparser-util.la part/libpart.la tpl/libtpl.la
