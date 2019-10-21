#!/bin/bash

set -e

if [[ $KEYEDGE_DIR = "" ]]; then
	echo "KEYEDGE_DIR is not set"
	exit 1
fi

$KEYEDGE_DIR/bin/keyedge keyedge/ocalls.h
mv ocalls.fbs keyedge/
$KEYEDGE_DIR/flatcc/bin/flatcc -a keyedge/ocalls.fbs
mv *.h keyedge/

./vault.sh

