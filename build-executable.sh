#!/bin/bash

mkdir build
cd build

cmake .. -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release
make -j4
