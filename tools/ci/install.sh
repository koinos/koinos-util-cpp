#!/bin/bash

if [ "$RUN_TYPE" = "coverage" ]; then
   sudo gem install coveralls-lcov
fi
