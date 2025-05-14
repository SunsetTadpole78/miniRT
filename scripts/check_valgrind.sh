#!/bin/bash

TMPFILE=$(mktemp)

valgrind ./miniRT maps/c 2> "$TMPFILE" &
PID=$!

sleep 5
xdotool key Escape
wait $PID

output=$(grep "total heap usage" "$TMPFILE" | grep -oP '\d+ allocs, \d+ frees')

rm "$TMPFILE"

allocs=$(echo "$output" | grep -oP '^\d+')
frees=$(echo "$output" | grep -oP '\d+(?= frees)')

if [[ "$allocs" == "$frees" ]]; then
    echo "OK"
    exit 0
else
    echo "KO: $allocs allocs, $frees frees"
    exit 1
fi

