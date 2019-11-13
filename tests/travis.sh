#!/bin/bash

cd ${KEYEDGE_DIR}/tests
rm result.log

for dir in ${KEYEDGE_DIR}/tests/*/
do
	./launch.sh $(basename ${dir})
done

cat result.log

