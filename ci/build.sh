#!/bin/bash

set -e
set -x

mkdir build
cd build

if [ "$RUN_TYPE" = "test" ]; then
   cmake -DCMAKE_BUILD_TYPE=Release -DHUNTER_RUN_UPLOAD=ON -DBUILD_TESTS=ON ..
   cmake --build . --config Release --parallel 3
elif [ "$RUN_TYPE" = "coverage" ]; then
   cmake -DCMAKE_BUILD_TYPE=Debug -DHUNTER_RUN_UPLOAD=ON -DBUILD_TESTS=ON -DCOVERAGE=ON ..
   cmake --build . --config Debug --parallel 3 --target coverage
fi
