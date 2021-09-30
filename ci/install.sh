#!/bin/bash

sudo apt-get install -yq --allow-downgrades libc6=2.31-0ubuntu9.2 libc6-dev=2.31-0ubuntu9.2
sudo -E apt-get -yq --no-install-suggests --no-install-recommends --allow-downgrades --allow-remove-essential --allow-change-held-packages install clang-11 llvm-11 -o Debug::pkgProblemResolver=yes

if [ "$RUN_TYPE" = "coverage" ]; then
   sudo apt-get install -y lcov ruby
   sudo gem install coveralls-lcov
fi
