#!/bin/sh

ERR_FILE=./kernel/test006.err

### Execute

./kernel/test006 2> $ERR_FILE
RET=$?
[ $RET -ne 0 ] && exit $RET

### Check err

grep -q 'zombie deletion' $ERR_FILE || exit 1

exit 0
