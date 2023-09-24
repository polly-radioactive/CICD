#!/bin/bash


if (($# != 1)); then
	echo "No filename!"
	exit 0
fi

filename=$1
flags=(b e n s t v)

#rm s21_cat
#make s21_cat

for flag in ${flags[*]};
do
	echo "--$flag"
	cat -$flag $filename >> out1.txt
	./s21_cat -$flag $filename >> out2.txt
	result=$(diff out1.txt out2.txt)
	if (($? == '0')); then
		echo "SUCCESS"
		rm out*
	else
		echo "FAIL"
		exit 0
	fi
done
