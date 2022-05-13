#! /bin/sh

SCORES="$HOME/.2048"
TMPFILE=$(mktemp)

if [ -e "$SCORES" ]
then
	echo "$1" >> "$SCORES"
	sort -rn < "$SCORES" | sed 20q > "$TMPFILE"
else
	echo "$1" > "$TMPFILE"
fi

mv "$TMPFILE" "$SCORES"
