#!/bin/bash

set -e
set -x

mkdir build
pushd build

if [ "$RUN_TYPE" = "test" ]; then
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake --build . --config Release --parallel 3
elif [ "$RUN_TYPE" = "coverage" ]; then
   cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON ..
   cmake --build . --config Debug --parallel 3 --target coverage
   lcov --version
elif [ "$RUN_TYPE" = "static-analysis" ]; then
   cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_ANALYSIS=ON ..
   cmake --build . --config Debug --parallel 3
elif [ "$RUN_TYPE" = "sanitizer" ]; then
   ls -la /usr/lib
   sudo ln /usr/lib/LLVMgold.so /usr/local/clang-16.0.0/lib/LLVMgold.so
   ls -la /usr/local/clang-16.0.0/lib
   ls -la /usr/local/clang-16.0.0/bin/../lib/

   popd
   mkdir build-address
   pushd build-address
   cmake -DCMAKE_BUILD_TYPE=Release -DSANITIZER=Address ..
   cmake --build . --config Release --parallel 3

   popd
   mkdir build-stack
   pushd build-stack
   cmake -DCMAKE_BUILD_TYPE=Release -DSANITIZER=Stack ..
   cmake --build . --config Release --parallel 3

   popd
   mkdir build-thread
   pushd build-thread
   cmake -DCMAKE_BUILD_TYPE=Release -DSANITIZER=Thread ..
   cmake --build . --config Release --parallel 3
fi
