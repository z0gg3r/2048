#! /bin/sh
if [ -e SCORES ]
then
	echo "$1\n" >> SCORES
	cat SCORES | sort -r | sed 20q
else
	echo "$1\n" > SCORES
fi
