rebuild "$@"

if [[ "$rebuild" == "true" ]]; then
  rm -rf ./build
fi
mkdir -p ./build

cd ./build
cmake ..
make

ctest --rerun-failed --output-on-failure
