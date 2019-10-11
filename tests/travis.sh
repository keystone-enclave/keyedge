#!/bin/bash

export HOST_PORT=${HOST_PORT:="$((3000 + RANDOM % 3000))"}

cd ${KEYEDGE_DIR}/tests
rm result.log

for dir in ${KEYEDGE_DIR}/tests/*/
do
	export TEST_FILE_NAME=$(basename ${dir})
	cd ${dir}
	# Try to build
	./keyedge_vault.sh
	# Launch QEMU test
	cd ${KEYSTONE_SDK_DIR}/../
	make image
	screen -L -dmS qemu ${KEYSTONE_SDK_DIR}/../scripts/run-qemu.sh
	sleep 10
	cd ${dir}
	${KEYEDGE_DIR}/tests/test.sh
	./clean.sh
	
	diff output.log expected
	if [ $? -eq 0 ]
	then
		echo "[PASS] output.log matches with the expected output for ${TEST_FILE_NAME}." >> ../result.log
	else
		echo "[FAIL] output.log does not match with the expected output for ${TEST_FILE_NAME}." >> ../result.log
	fi
	
	cd ..
done

cat result.log

