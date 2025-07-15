rm -rf ./build
mkdir -p ./build

cd ./build
cmake ..
make

# ctest --rerun-failed --output-on-failure

./test_kv_cli
