set -e

# Set up environment
cp Makefile-eapp $1/eapp/Makefile
cp app.lds $1/eapp/app.lds
cp host_launcher.cpp $1/host/host_launcher.cpp
cp Makefile-host $1/host/Makefile
cp vault.sh $1/vault.sh
cp keyedge_vault.sh $1/keyedge_vault.sh
cp clean.sh $1/clean.sh

cd ${KEYEDGE_DIR}/tests/$1
# Try to build
./keyedge_vault.sh $1
./clean.sh

rm eapp/Makefile
rm eapp/app.lds
rm host/Makefile
rm host/host_launcher.cpp
rm vault.sh
rm keyedge_vault.sh
rm clean.sh

