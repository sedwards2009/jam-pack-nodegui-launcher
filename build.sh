#!/bin/bash

rm -rf build || true

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
