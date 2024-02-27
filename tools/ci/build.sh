#!/bin/bash

set -e
set -x

if [ "$RUN_TYPE" = "test" ]; then
   mkdir build
   pushd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake --build . --config Release --parallel 3
elif [ "$RUN_TYPE" = "coverage" ]; then
   mkdir build
   pushd build
   cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON ..
   cmake --build . --config Debug --parallel 3 --target coverage
elif [ "$RUN_TYPE" = "static-analysis" ]; then
   mkdir build
   pushd build
   cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_ANALYSIS=ON ..
   cmake --build . --config Debug --parallel 3
elif [ "$RUN_TYPE" = "sanitizer" ]; then
   mkdir build-address
   pushd build-address
   cmake -DCMAKE_BUILD_TYPE=Debug -DSANITIZER=Address ..
   cmake --build . --config Debug --parallel 3

   popd
   mkdir build-stack
   pushd build-stack
   cmake -DCMAKE_BUILD_TYPE=Debug -DSANITIZER=Stack ..
   cmake --build . --config Debug --parallel 3

   popd
   mkdir build-thread
   pushd build-thread
   cmake -DCMAKE_BUILD_TYPE=Debug -DSANITIZER=Thread ..
   cmake --build . --config Debug --parallel 3
fi
