#!/bin/sh

THIS_HOME=$(dirname $0)
CMAP_DIR=$THIS_HOME/..

ECHO="echo -e"
_R_="\e[31m"
_G_="\e[32m"
___="\e[0m"

do_rm() {
	local ELMT_LIST=$1
	$ECHO "${_G_}REMOVE${___}: [$ELMT_LIST]"
	rm -r $ELMT_LIST
}

process_pattern() {
	local PATTERN=$1
	local ELMT_LIST=KO
	local TMP=$(echo "$PATTERN" | cut -c1)
	if [ "$TMP" = "/" ]
	then
		ELMT_LIST=$(ls $CMAP_DIR$PATTERN 2> /dev/null | xargs)
	else
		TMP=$(echo "$PATTERN" | tr '/' '_')
		[ "$TMP" = "$PATTERN" ] &&
			ELMT_LIST=$(find $CMAP_DIR -name "$PATTERN" \
				2> /dev/null | xargs)
	fi
	if [ "$ELMT_LIST" = "KO" ]
	then
		$ECHO "${_R_}SKIP${___}: [$PATTERN]"
	else
		[ -n "$ELMT_LIST" ] && do_rm "$ELMT_LIST"
	fi
}

cat $CMAP_DIR/.gitignore | while read
do
	[ -n "$REPLY" ] && process_pattern "$REPLY"
done
