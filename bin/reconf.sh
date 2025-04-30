#!/bin/sh

cd $(dirname $0)/..

touch ./config.rpath

mkdir -p m4
autoreconf -i -v
