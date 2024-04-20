#!/bin/bash

kernel_release="$(uname -r)"

if [[ "$kernel_release" == *"Microsoft"* || "$kernel_release" == *"microsoft"* ]]; then
    is_windows=true
fi

# Read the compile_commands.json file and extract the "id" values into an array
readarray -t ids < <(sed -n 's/.*"id":\s*"\([^"]*\)".*/\1/p' compile_commands.json)

# Check if an id parameter is provided
if [ -z "$1" ]; then
    echo "Error: No id provided. Usage: $0 <id>"
    exit 1
fi

# Initialize a variable to store the desired command
desired_command=""

# Loop through the ids and find the one that matches the provided parameter
for id in "${ids[@]}"; do
    if [ "${id}" == "${1}" ]; then
        # Extract the command for the matching id
        desired_command=$(sed -n "/\"id\":\s*\"${id}\"/,/}/ {/\"command\":\s*\"/,/\"/p}" compile_commands.json | sed -n 's/.*"\(.*\)".*/\1/p')
        break
    fi
done

# Check if the desired command was found
if [ -z "${desired_command}" ]; then
    echo "Error: No command found with id ${1}"
    exit 1
fi

# Extract the output executable name
output_name=$(echo "${desired_command}" | sed 's/.*-o \([^ ]*\).*/\1/' | head -n 1)
output_path=$(echo $output_name | sed 's|/[^/]*$||')

if [ ! -d "$output_path" ]; then
    mkdir -p "$output_path"
fi

# Compile the program
echo -e "[INFO]: Compiling: ${id}"

if [[ $is_windows ]]; then
    cmd.exe /C "$desired_command"
else
    "./${desired_command}"
fi

if [[ $is_windows ]]; then
    output_extension=".exe"
else
    output_extension=".out"
fi

output_file=$output_name$output_extension

# Run the compiled executable
if [ $? -eq 0 ]; then
    echo "[INFO]: Running output file: ./$output_file"

    eval "./$output_file"
else
    echo "[ERROR]: Compilation failed"
fi
