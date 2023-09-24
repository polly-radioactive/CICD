#!/bin/bash

cd cat
make clean
make
bash cat_tests.sh test.txt >>  test.tmp
bash cat_tests.sh test_1_cat.txt >>  test.tmp
bash cat_tests.sh test_2_cat.txt >>  test.tmp
bash cat_tests.sh test_3_cat.txt >>  test.tmp
bash cat_tests.sh test_4_cat.txt >>  test.tmp
bash cat_tests.sh test_5_cat.txt >>  test.tmp
bash cat_tests.sh test_case_cat.txt >>  test.tmp
count=$( grep -o -i FAIL test.tmp | wc -l )
rm test.tmp
if [[ $count != 0 ]]; then
  echo "test_cat: Fail"
  exit 1
else
  echo "test_cat: Success"
  exit 0
fi
