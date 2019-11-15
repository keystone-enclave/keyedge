export HOST_PORT=${HOST_PORT:="$((3000 + RANDOM % 3000))"}
cd ${KEYSTONE_SDK_DIR}/../
screen -L -dmS qemu ${KEYSTONE_SDK_DIR}/../scripts/run-qemu.sh
sleep 10

cd ${KEYEDGE_DIR}/tests/$1
../test.sh $1
	
diff output.log expected
if [ $? -eq 0 ]
then
	echo "[PASS] output.log matches with the expected output for $1." >> ../result.log
else
	echo "[FAIL] output.log does not match with the expected output for $1." >> ../result.log
fi

cd ..

