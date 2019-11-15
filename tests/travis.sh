#!/bin/bash

cd ${KEYEDGE_DIR}/tests
rm result.log

set -e

for dir in ${KEYEDGE_DIR}/tests/*/
do
	./build.sh $(basename ${dir})
done

cd ${KEYSTONE_SDK_DIR}/../
make image
cd ${KEYEDGE_DIR}/tests

set +e

for dir in ${KEYEDGE_DIR}/tests/*/
do
	./launch.sh $(basename ${dir})
done

cat result.log

