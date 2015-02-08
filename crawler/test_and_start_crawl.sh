#!/bin/bash

echo -e ***********TESTING INPUT VALIDATION***************\n

echo -e What if the URL is invalid?
echo -e Testing for URL ww.cs.dartmouth.edu, path ./webpages, depth 1
make -s clean
make -s
./crawler www.cs.dartmouth.edu ./webpages 1
echo

echo "What if the path doesn't exist?"
echo Testing for URL www.cs.dartmouth.edu, path ./badfolder, depth 1\n
./crawler www.cs.dartmouth.edu ./badfolder 1
echo

echo "What if the depth is greater than 4?"
echo Testing for URL www.cs.dartmouth.edu, path ./webpages, depth 5
./crawler www.cs.dartmouth.edu ./webpages 5
echo

echo "Now crawling http://www.cs.dartmouth.edu at depth 1."
make -s clean
make -s
./crawler http://www.cs.dartmouth.edu ../webpages 1
echo -e "Finished crawling at depth 1 \n"

echo "Now crawling http://www.cs.dartmouth.edu at depth 3. Please wait. You can check the status of crawler by monitoring ./webpages"
make -s clean
make -s
./crawler http://www.cs.dartmouth.edu ../webpages 3
echo

echo "Crawler also works at depth 4, but I'll leave it up to you to see if you are curious."