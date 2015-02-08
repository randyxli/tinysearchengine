#!/bin/bash

make -s clean
make -s test

echo *************RUNNING TEST HARNESS*************
./@test

echo -e ***********TESTING INPUT VALIDATION***************\n

echo "What if the index file doesn't exist?"
echo Testing for path ./badfile
./query ./badfile
echo

echo "What if the file exists but is not an index file?"
echo Testing for path ./query.c
./query ./query.c
echo

echo "Now starting the query engine:"
make -s clean
make -s
./query ../data/index.dat