#! /bin/sh

SCORES="$HOME/.2048"

if [ -e "$SCORES" ]
then
	echo "$1" >> "$SCORES"
	sort -r < "$SCORES" | sed 20q > "$SCORES.new"
	mv "$SCORES.new" "$SCORES"
else
	echo "$1" > "$SCORES"
fi
