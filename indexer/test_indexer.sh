#!/bin/bash

make -s clean
make -s

echo -e ***********TESTING INPUT VALIDATION***************\n

echo "What if the path doesn't exist?"
echo Testing for path ./badfolder
./indexer ./badfolder
echo

echo "What if only one argument is given but it is not a directory?"
echo Testing for path ./indexer.c
./indexer ./indexer.c
echo

echo "What if two arguments are given but the first is not a .dat file?"
echo Testing for path ./indexer.c, path ./newindex.dat
./indexer ./indexer.c
echo

echo "What if two arguments are given but the second is not a valid path?"
echo Testing for path ./index.dat, path ./badfolder/nofile
./indexer ./indexer.c
echo

echo "Now indexing ./webpages. The output file is ./index.dat. Please wait"
make -s clean
make -s
./indexer ../webpages/
echo "Finished indexing ./webpages"

echo "Now reconstructing inverted list ./index.dat. The output file is ./backwardsindex.dat. Note that my data structure design causes all the linked lists in the inverted list to be reconstructed in backwards order. Please wait."
./indexer ../data/index.dat ./backwardsindex.dat
echo "Finished rebuilding ./index.dat."
echo

echo "Now reconstructing inverted list ./backwardsindex.dat. The output file is ./index1.dat. This reconstruction will prove backwardsindex.dat has indentical data to index.dat, except backwards. Please wait."
./indexer ./backwardsindex.dat ./index1.dat
echo "Finished rebuilding ./backwardsindex.dat."
echo

echo "Now calling bash command diff -s, which will report when two files are identical."
diff -s ../data/index.dat ./index1.dat