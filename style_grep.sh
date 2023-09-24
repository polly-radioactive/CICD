#!/bin/bash

cd grep
make check_style 2> style.tmp
count=$( cat style.tmp | wc -l )
if [[ $count != 0 ]]; then
 cat style.tmp
 exit 1
else
  exit 0
fi
rm style.tmp
