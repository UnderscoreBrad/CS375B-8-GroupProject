#!/bin/sh
# Runs all the tests in constraints.txt

linecount=$(wc -l < constraints.txt)

for (( c=1; c<=$linecount + 1; c++ ))
do
	echo "./hashing_test $c"
	echo
	./hashing_test $i
	echo
done
