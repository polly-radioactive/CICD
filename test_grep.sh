#!/bin/bash

cd grep
make clean
make
bash test_func_grep.sh >>  test.tmp
count=$( grep -o -i FAIL test.tmp | wc -l )
rm test.tmp
if [[ $count != 1 ]]; then
  echo "test_grep: Fail"
  exit 1
else
  echo "test_grep: Success"
  exit 0
fi
