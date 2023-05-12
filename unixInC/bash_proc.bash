#!/bin/bash

if [ $# -eq 1 ]
then
	F=$1
	out=$(gcc -Wall -o ${F%.c} $F 2>&1)
	warnings=$(echo "$out" | grep "warning:" | wc -l)
	error=$(echo "$out" | grep "error:" | wc -l)
	echo -e "$warnings, $error, $F"
fi
