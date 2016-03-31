#!/bin/sh

TEST_DESC_KEY="TEST_DESC"

grep --color -r "@$TEST_DESC_KEY@" $(dirname $0)
