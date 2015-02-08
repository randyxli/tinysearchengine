#!/bin/bash
#
# Script: wget.sh
#
# Description: writes the contents of a webpage to file currentpage.html
#
# Input: a URL
#
# Output: The contents of the page specified by the URL in file currentpage.html

wget -t 3 -qO "currentpage.html" $1
