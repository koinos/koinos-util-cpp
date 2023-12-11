#!/bin/bash

sudo apt install -y clang llvm

if [ "$RUN_TYPE" = "coverage" ]; then
   sudo apt-get install -y lcov ruby
   sudo gem install coveralls-lcov
fi
