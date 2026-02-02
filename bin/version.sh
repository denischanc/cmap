#!/bin/bash

THIS_HOME=$(dirname $0)
CMAP_DIR=$THIS_HOME/..

grep "^AC_INIT(" $CMAP_DIR/configure.ac | \
	awk -F[ '{print $3}' | \
	awk -F] '{print $1}'
