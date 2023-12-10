#!/bin/bash
set -eu

days=(1 2)
for day in "${days[@]}"; do
    for part in 1 2; do
        echo "+ day${day} part${part}..."
        ./run "$day/part${part}.cc" "${day}/input.txt"
    done
done
