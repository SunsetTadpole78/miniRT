#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;91m"
RESET="\033[0m"

if [[ ! $# -eq 2 || ! "$2" =~ ^-?[0-9]+([.][0-9]+)?$ ]]; then
	echo -e "${RED}You must specified .rt map path and time before press escape key ${RESET}"
	exit 1
fi

TMPFILE=$(mktemp)

echo "Launching valgrind..."
valgrind ./miniRT $1 2> "$TMPFILE" &
PID=$!

sleep $2
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
    echo -e "${GREEN}Leaks OK${RESET}"
else
    echo -e "${RED}Leaks KO: $allocs allocs, $frees frees${RESET}"
    code=1
fi

if [[ "$output2" == "0" ]]; then
	echo -e "${GREEN}Conditional jump or move OK${RESET}"
else
	echo -e "${RED}Conditional jump or move KO: $output2 errors${RESET}"
	code=1
fi

exit "$code"
