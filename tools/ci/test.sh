#!/bin/bash

set -e
set -x

if [ "$RUN_TYPE" = "test" ]; then
   cd $TRAVIS_BUILD_DIR/build
   cmake --build . --config Release --parallel 3 --target format.check

   cd $TRAVIS_BUILD_DIR/build/tests
   exec ctest -j3 --output-on-failure
elif [ "$RUN_TYPE" = "sanitizer" ]; then
   pushd $TRAVIS_BUILD_DIR/build-address/tests
   exec ctest -j1 --output-on-failure

   popd
   pushd $TRAVIS_BUILD_DIR/build-stack/tests
   exec ctest -j1 --output-on-failure

   popd
   pushd $TRAVIS_BUILD_DIR/build-thread/tests
   exec ctest -j1 --output-on-failure
fi
