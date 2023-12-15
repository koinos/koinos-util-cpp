#!/bin/bash

set -e
set -x

if [ "$RUN_TYPE" = "test" ]; then
   cd $TRAVIS_BUILD_DIR/build
   cmake --build . --config Release --parallel 3 --target format.check

   cd $TRAVIS_BUILD_DIR/build/tests
   ctest -j3 --output-on-failure
elif [ "$RUN_TYPE" = "sanitizer" ]; then
   cd $TRAVIS_BUILD_DIR/build-address/tests
   ctest -j1 --output-on-failure

   cd $TRAVIS_BUILD_DIR/build-stack/tests
   ctest -j1 --output-on-failure

   cd $TRAVIS_BUILD_DIR/build-thread/tests
   ctest -j1 --output-on-failure
fi
