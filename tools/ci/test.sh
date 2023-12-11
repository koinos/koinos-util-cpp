#!/bin/bash

set -e
set -x

if [ "$RUN_TYPE" = "test" ]; then

   if [ "$CXX" = "clang++" ]; then
      cd $TRAVIS_BUILD_DIR/build
      cmake --build . --config Release --parallel 3 --target format.check
   fi

   cd $TRAVIS_BUILD_DIR/build/tests
   exec ctest -j3 --output-on-failure
fi

