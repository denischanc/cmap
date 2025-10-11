#!/bin/sh

grep --color -r "@TEST_DESC@" $(dirname $0)/../test
