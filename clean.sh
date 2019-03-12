#!/bin/bash
# Script to clean the tree from all compiled files.

# Remove the dedicated output directories
cd `dirname $0`

rm -rf build

# We're done!
echo Cleaned up the project!
