#!/bin/bash

# List of main folders
directories=("level" "bonus")

# Create directories and required files
for dir in "${directories[@]}"; do
    if [ "$dir" == "level" ]; then
        max=9
    else
        max=3
    fi

    for i in $(seq 0 $max); do
        folder_name="${dir}${i}"
        mkdir -p "$folder_name/Ressources"
        touch "$folder_name/flag"
        touch "$folder_name/source.c"
        touch "$folder_name/walkthrough.md"
    done

done

echo "Folders and files created successfully."
