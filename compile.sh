rebuild "$@"

if [[ "$rebuild" == "true" ]]; then
  rm -rf ./build
fi
mkdir -p ./build

cd ./build
cmake ..
make

./test_kv_cli

# ctest --rerun-failed --output-on-failure
