#!/bin/bash

set -e

if [[ $KEYEDGE_DIR = "" ]]; then
	echo "KEYEDGE_DIR is not set"
	exit 1
fi

if [[ $FLATCC_DIR = "" ]]; then
	echo "FLATCC_DIR is not set"
	exit 1
fi

$KEYEDGE_DIR/bin/keyedge keyedge/ocalls.h
mv ocalls.fbs keyedge/
$FLATCC_DIR/bin/flatcc -a keyedge/ocalls.fbs
mv *.h keyedge/

./vault.sh

