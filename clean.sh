#!/bin/bash
# This script deletes all files with the extensions: .am, .ob, .ext, and .ent
# Use with caution!

echo "Cleaning everything"
echo

find . -type f \( -name "*.am" -o -name "*.ob" -o -name "*.ext" -o -name "*.ent" \) -exec rm -f {} \;

make clean
echo
echo "Cleaning completed."
