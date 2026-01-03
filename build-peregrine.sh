#!/bin/bash

cd peregrine || exit
source tbb2020/bin/tbbvars.sh intel64
make -j CC=g++