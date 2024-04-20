#!/bin/bash

# Check if the id parameter is provided
if [ -z "$1" ]; then
    echo "Error: No id provided. Usage: $0 <id>"
    exit 1
fi

# Read the compile_commands.json file
readarray -t commands < compile_commands.json

# Initialize a variable to store the desired command
desired_command=""

# Loop through the commands and find the one with the matching id
for command in "${commands[@]}"; do
    id=$(echo "${command}" | sed 's/.*"id":"\([^"]*\)".*/\1/')
    if [ "${id}" == "${1}" ]; then
        desired_command="${command}"
        break
    fi
done

# Check if the desired command was found
if [ -z "${desired_command}" ]; then
    echo "Error: No command found with id ${1}"
    exit 1
fi

# Extract the "command" field
cmd_str=$(echo "${desired_command}" | sed 's/.*"command":"\([^"]*\)".*/\1/')

# Extract the output executable name
output=$(echo "${cmd_str}" | sed 's/.*-o \([^ ]*\).*/\1/')

# Compile the program
echo "Compiling: ${cmd_str}"
eval "${cmd_str}"

# Run the compiled executable
if [ $? -eq 0 ]; then
    echo "Running: ${output}"
    "./${output}"
else
    echo "Compilation failed."
fi