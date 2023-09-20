
BUILT_SOURCES = cmap-parser.h

AM_LFLAGS = --prefix=cmap_parser_
LEX_OUTPUT_ROOT = lex.cmap_parser_

AM_YFLAGS = -d --name-prefix=cmap_parser_
### AM_YFLAGS = -d --name-prefix=cmap_parser_ -Wcounterexamples

noinst_HEADERS = \
  cmap-scanner.h \
  cmap-parser-string.h \
  cmap-parser-util.h \
  cmap-parser-util-define.h \
  cmap-parser-part.h

bin_PROGRAMS = cmap-parser

cmap_parser_SOURCES = \
  cmap-scanner.l \
  cmap-parser.y \
  cmap-parser-string.c \
  cmap-parser-util.c \
  cmap-parser-part.c \
  cmap-main.c
cmap_CFLAGS = -Wall
