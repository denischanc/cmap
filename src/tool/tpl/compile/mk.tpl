
MAKEFLAGS += --no-print-directory

CFLAGS = -Wall -I@CMAP_INCLUDEDIR@%s
LDFLAGS = -L@CMAP_LIBDIR@ -lcmap

%s:%s
	@gcc $(CFLAGS) $(LDFLAGS) -o $@ $^
