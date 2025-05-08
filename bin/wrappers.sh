#!/bin/sh

WRAPPERS_DIR=$(realpath $(dirname $0))
SRC_DIR=$(realpath $WRAPPERS_DIR/../src)
SCORE_DIR=$(realpath $SRC_DIR/core/score)
KERNEL_DIR=$(realpath $SRC_DIR/kernel)

types() {
	local TYPE_ROOT=$1
	local FILE=$2
	local NAME TYPE_NESTED
	grep "macro(" $FILE | grep -v _LOOP | \
	awk -F\( '{print $2}' | awk -F\) '{print $1}' | \
	awk -F, '{print $1" "$3}' | while read NAME TYPE_NESTED
	do
		local TYPE=CMAP_${TYPE_ROOT}_$NAME
		echo "[[ $TYPE ($TYPE_NESTED) ]]"
		cd $SRC_DIR && grep -rl $TYPE .
	done
}

types SSET $SCORE_DIR/cmap-sset-define.h
types SLIST $SCORE_DIR/cmap-slist-define.h
types POOL $KERNEL_DIR/cmap-pool-handler-define.h
