#!/bin/bash

for i in `ls *.txt`
do
	AUX=`grep market $i`
	if [ -z "$AUX" ]
	then
		echo "1" >> $i
		echo "market" >> $i
	fi
done
