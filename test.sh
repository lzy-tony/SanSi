#!/bin/bash

make
./srcgen ./test/test1.txt 256 128000
./main ./test/test1.txt ./test/test1.out
./toBin ./test/test1.out ./test/test1.bin