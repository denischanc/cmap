#!/bin/sh

OUT_FILE=./kernel/test003.out
ERR_FILE=./kernel/test003.err

### Execute

./kernel/test003 > $OUT_FILE 2> $ERR_FILE
RET=$?
[ $RET -ne 0 ] && exit $RET

### Check out

[ $(wc -l $OUT_FILE | awk '{print $1}') -ne 3 ] && exit 1
grep -q 'Hello World !!!' $OUT_FILE || exit 1

### Check err

grep -q '^\[ERROR\]$' $ERR_FILE || exit 1

exit 0
