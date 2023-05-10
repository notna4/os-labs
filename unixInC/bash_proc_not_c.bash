#!/bin/bash

if [ $# -eq 1 ]
then
	lines=$(wc -l $1)
	
	echo "Lines in $1: $lines"
fi
