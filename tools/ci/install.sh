#!/bin/bash

wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
echo | sudo ./llvm.sh 16 all

if [ "$RUN_TYPE" = "coverage" ]; then
   sudo gem install coveralls-lcov
fi
