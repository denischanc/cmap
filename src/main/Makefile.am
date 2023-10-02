
BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_
### AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-file-util.h \
  cmap-gen.h \
  cmap-kv.h \
  cmap-parser-part.h \
  cmap-parser-util-define.h \
  cmap-parser-util.h \
  cmap-prj.h \
  cmap-scanner.h \
  cmap-string.h

bin_PROGRAMS = cmap-main

cmap_main_SOURCES = \
  cmap-file-util.c \
  cmap-gen.c \
  cmap-kv.c \
  cmap-main.c \
  cmap-parser-part.c \
  cmap-parser-util.c \
  cmap-parser.y \
  cmap-prj.c \
  cmap-scanner.l \
  cmap-string.c
cmap_main_CFLAGS = \
  -Wall \
  -DCMAP_INSTALL_BINDIR="\"$(bindir)\"" \
  -DCMAP_INSTALL_LIBDIR="\"$(libdir)\"" \
  -DCMAP_INSTALL_INCLUDEDIR="\"$(includedir)\""
