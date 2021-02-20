#! /bin/sh
if [ -e SCORES ]
then
	echo "\n$1" >> SCORES
	cat SCORES | sort -r | sed 20q
else
	echo "$1" > SCORES
fi
