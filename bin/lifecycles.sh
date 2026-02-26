#!/bin/sh

LIFECYCLES_DIR=$(realpath $(dirname $0))
SRC_DIR=$(realpath $LIFECYCLES_DIR/../src)

lifecycle_types() {
	local LC_SUPER=$1
	local FILE
	cd $SRC_DIR && grep -r "$LC_SUPER super;" . | awk -F: '{print $1}' | \
	while read FILE
	do
		local LC_TYPE=$(basename $FILE | \
			sed -e 's/-define//' -e 's/\.c//' -e 's/\.h//' | \
			tr '[a-z]-' '[A-Z]_')
		echo "$LC_TYPE [$FILE]"
		lifecycle_types $LC_TYPE
	done
}

lifecycle_types CMAP_LIFECYCLE
