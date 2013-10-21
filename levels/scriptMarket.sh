#!/bin/bash

for i in `ls`
do
	AUX=`grep market $i`
	if [ -z "$i" ]
	then
		echo "1" >> $i
		echo "market" >> $i
	fi
done
