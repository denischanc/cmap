
BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-clean.h \
  cmap-file-util.h \
  cmap-fn-name-define.h \
  cmap-fn-name.h \
  cmap-gen.h \
  cmap-gen-main.h \
  cmap-kv.h \
  cmap-option-define.h \
  cmap-option.h \
  cmap-parser-util-define.h \
  cmap-part.h \
  cmap-parser-util.h \
  cmap-prj.h \
  cmap-scanner.h \
  cmap-string.h

bin_PROGRAMS = cmap-tool

cmap_tool_SOURCES = \
  cmap-clean.c \
  cmap-file-util.c \
  cmap-fn-name.c \
  cmap-gen.c \
  cmap-gen-main.c \
  cmap-kv.c \
  cmap-option.c \
  cmap-parser-util.c \
  cmap-part.c \
  cmap-parser.y \
  cmap-prj.c \
  cmap-scanner.l \
  cmap-string.c \
  cmap-tool.c
cmap_tool_CFLAGS = \
  -Wall \
  -DCMAP_INSTALL_BINDIR="\"$(bindir)\"" \
  -DCMAP_INSTALL_LIBDIR="\"$(libdir)\"" \
  -DCMAP_INSTALL_INCLUDEDIR="\"$(includedir)\""
