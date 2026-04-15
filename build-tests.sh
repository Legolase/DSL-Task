#!/bin/bash

mkdir build
cd build

cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
make -j4
