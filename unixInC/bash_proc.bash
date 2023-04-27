#!/bin/bash

if [ $# -eq 1 ]
then
	echo "$1"
	F=$1
	out=$(gcc -c -Wall -o ${F%.c} $F 2>&1)
	warnings=$(echo $out | grep "warning:" | wc -l)
	error=$(echo $out | grep "error:" | wc -l)
	echo "warnings: $warnings"
	echo "errors: $error"
fi
