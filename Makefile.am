
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src test

wc:
	wc -l $$(find $(srcdir)/src -name "*.c" -o -name "*.h")
	wc -l $$(find $(srcdir)/test -name "*.c" -o -name "*.h")

test_desc:
	$(srcdir)/test/test_desc.sh

.PHONY: wc test_desc
