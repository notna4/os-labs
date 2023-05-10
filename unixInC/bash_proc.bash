#!/bin/bash

if [ $# -eq 1 ]
then
	F=$1
	touch "grades.txt"
	out=$(gcc -Wall -o ${F%.c} $F 2>&1)
	warnings=$(echo "$out" | grep "warning:" | wc -l)
	error=$(echo "$out" | grep "error:" | wc -l)
	echo -e "$warnings \t $error"

	score=0

	if [ "$warnings" -eq 0 ] && [ "$error" -eq 0 ]
	then
		score=10
	elif [ "$error" -gt 0 ]
	then
		score=1
	elif [ "$error" -eq 0 ] && [ "$warnings" -gt 9 ]
	then
		score=2
	elif [ "$error" -eq 0 ] && [ "$warnings" -lt 11 ]
	then
		score=$((2+8*(10-$warnings)/10))
	fi
	echo "$F: $score" > "grades.txt"
fi
