#!/bin/bash

echo **************************TESTING CRAWLER********************
cd crawler
./test_and_start_crawl.sh

echo **************************TESTING INDEXER********************
cd ../indexer
./test_indexer.sh

echo *************************TESTING QUERY ENGINE****************
cd ../queryengine
./test_query.sh

echo Done. It was a pleasure!
