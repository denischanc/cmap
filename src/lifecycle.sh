#!/bin/sh

THIS_DIR=$(dirname $0)

lifecycles() {
	local LC=$1
	local FILE
	grep -r "$LC super;" $THIS_DIR | awk -F: '{print $1}' | \
	while read FILE
	do
		LC_O=$(basename $FILE .h | sed -e 's/-define//' | tr '[a-z]-' '[A-Z]_')
		echo "$LC_O [$FILE]"
		lifecycles $LC_O
	done
}

lifecycles CMAP_LIFECYCLE
