#!/bin/sh

if [ -d cmake-build-debug ]; then
	rm -rf cmake-build-debug
fi

mkdir cmake-build-debug
cd cmake-build-debug

cmake .. -DCMAKE_BUILD_TYPE=DEBUG

cd ..

if [ -d cmake-build-release ]; then
	rm -rf cmake-build-release
fi

mkdir cmake-build-release
cd cmake-build-release
cmake .. -DCMAKE_BUILD_TYPE=RELEASE

