mkdir -p ./build

cd ./build
cmake ..
make

# ctest --rerun-failed --output-on-failure

cd ..
./build/test_kv_cli
