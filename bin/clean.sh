#!/bin/sh

THIS_HOME=$(dirname $0)
CMAP_DIR=$THIS_HOME/..

do_rm() {
	local ELMT_LIST=$1
	if [ -n "$ELMT_LIST" ]
	then
		echo "REMOVE: [$ELMT_LIST]"
		rm -rf $ELMT_LIST
	fi
}

process_pattern() {
	local PATTERN=$1
	local ELMT_LIST=$(find $CMAP_DIR -name "$PATTERN" | tr '\n' ' ')
	do_rm "$ELMT_LIST"
}

cat $CMAP_DIR/.gitignore | while read PATTERN
do
	process_pattern "$PATTERN"
done
