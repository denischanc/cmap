
BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_
### AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-scanner.h \
  cmap-string.h \
  cmap-parser-util.h \
  cmap-parser-util-define.h \
  cmap-parser-part.h \
  cmap-main-usage.h

bin_PROGRAMS = cmap-main

cmap_main_SOURCES = \
  cmap-scanner.l \
  cmap-parser.y \
  cmap-string.c \
  cmap-parser-util.c \
  cmap-parser-part.c \
  cmap-main.c \
  cmap-main-usage.c
cmap_main_CFLAGS = -Wall
