#!/bin/bash

TMPFILE=$(mktemp)

echo "Launching valgrind..."
valgrind ./miniRT scripts/good_map.rt 2> "$TMPFILE" &
PID=$!

sleep 5
xdotool key Escape
wait $PID

output=$(grep "total heap usage" "$TMPFILE" | grep -oP '\d+ allocs, \d+ frees')
output2=$(grep "Conditional jump or move" "$TMPFILE" | wc -l)
code=0

cat "$TMPFILE"
rm "$TMPFILE"

allocs=$(echo "$output" | grep -oP '^\d+')
frees=$(echo "$output" | grep -oP '\d+(?= frees)')

if [[ "$allocs" == "$frees" ]]; then
    echo "Leaks OK"
else
    echo "Leaks KO: $allocs allocs, $frees frees"
    code=1
fi

if [[ "$output2" == "0" ]]; then
	echo "Conditional jump or move OK"
else
	echo "Conditional jump or move KO: $output2 errors"
	code=1
fi

exit "$code"
