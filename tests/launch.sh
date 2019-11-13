export HOST_PORT=${HOST_PORT:="$((3000 + RANDOM % 3000))"}

# Set up environment
cp Makefile-eapp $1/eapp/Makefile
cp app.lds $1/eapp/app.lds
cp Makefile-host $1/host/Makefile
cp vault.sh $1/vault.sh
cp keyedge_vault.sh $1/keyedge_vault.sh
cp clean.sh $1/clean.sh

cd ${KEYEDGE_DIR}/tests/$1
# Try to build
./keyedge_vault.sh $1
# Launch QEMU test
cd ${KEYSTONE_SDK_DIR}/../
make image
screen -L -dmS qemu ${KEYSTONE_SDK_DIR}/../scripts/run-qemu.sh
sleep 10

cd ${KEYEDGE_DIR}/tests/$1
../test.sh $1
./clean.sh

rm eapp/Makefile
rm eapp/app.lds
rm host/Makefile
rm vault.sh
rm keyedge_vault.sh
rm clean.sh
	
diff output.log expected
if [ $? -eq 0 ]
then
	echo "[PASS] output.log matches with the expected output for $1." >> ../result.log
else
	echo "[FAIL] output.log does not match with the expected output for $1." >> ../result.log
fi

cd ..

