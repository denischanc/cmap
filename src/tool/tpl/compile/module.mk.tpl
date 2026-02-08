
CFLAGS = -Wall -I@CMAP_INCLUDEDIR@%s
LDFLAGS =%s

%s:%s
	gcc -shared -fPIC $(CFLAGS) $(LDFLAGS) -o $@ $^
