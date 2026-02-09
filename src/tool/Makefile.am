
include $(top_srcdir)/src/tool/common.mk

SUBDIRS = parser part tpl

BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-build.h \
  cmap-build-main.h \
  cmap-cli-define.h \
  cmap-cli.h \
  cmap-compile.h \
  cmap-config-define.h \
  cmap-config.h \
  cmap-console-define.h \
  cmap-console.h \
  cmap-do-parse.h \
  cmap-exec.h \
  cmap-file-util.h \
  cmap-fn-name.h \
  cmap-part-define.h \
  cmap-part.h \
  cmap-prj.h \
  cmap-scanner.h \
  cmap-stack-define.h \
  cmap-string.h \
  cmap-strings.h \
  cmap-tool.h \
  cmap-usage.h \
  cmap-util.h

bin_PROGRAMS = cmap-tool

cmap_tool_SOURCES = \
  cmap-build.c \
  cmap-build-main.c \
  cmap-cli.c \
  cmap-compile.c \
  cmap-config.c \
  cmap-console.c \
  cmap-do-parse.c \
  cmap-exec.c \
  cmap-file-util.c \
  cmap-fn-name.c \
  cmap-parser.y \
  cmap-part.c \
  cmap-prj.c \
  cmap-scanner.l \
  cmap-string.c \
  cmap-strings.c \
  cmap-tool.c \
  cmap-usage.c \
  cmap-util.c
cmap_tool_LDADD = parser/libparser.la part/libpart.la tpl/libtpl.la
