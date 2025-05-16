#!/bin/bash

MAP_DIR="./maps/parsing"

for file_path in "$MAP_DIR"/*.rt; do
	file_name=$(basename "$file_path")

	[[ "$file_name" == "empty.rt" ]] && continue
	./scripts/check_valgrind.sh $file_path 0
	exit_code=$(echo "$?")
	if [[ ! $? -eq 0 ]]; then
		exit 1
	fi
done
