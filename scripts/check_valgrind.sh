#!/bin/bash

TMPFILE=$(mktemp)

echo "Launching valgrind..."
valgrind ./miniRT scripts/good_map.rt 2> "$TMPFILE" &
PID=$!

sleep 5
xdotool key Escape
wait $PID

output=$(grep "total heap usage" "$TMPFILE" | grep -oP '\d+ allocs, \d+ frees')

cat "$TMPFILE"
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

