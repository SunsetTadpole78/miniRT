#!/bin/bash

MAP_DIR="./maps/parsing"

for file_path in "$MAP_DIR"/*.rt; do
	file_name=$(basename "$file_path")

	./scripts/check_valgrind.sh $file_path 0
	exit_code=$?
	if [[ $exit_code -ne 0 ]]; then
	    exit 1
	fi
done
