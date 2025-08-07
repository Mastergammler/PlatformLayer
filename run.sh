#!/bin/bash

OUTPUT_DIR=".build"
EXEC_NAME=platform-demo.exe

./build.sh $OUTPUT_DIR $EXEC_NAME

if [ $? -eq 0 ]; then
    echo -e "\n---- POWERED BY WINE ----"

    # NOTE: wine searches in the directory relative to the executable!
    # not the current directory from which it was called
    # this is different to how the game itself searches for resources from the directory it was called
    wine $OUTPUT_DIR/$EXEC_NAME debug
fi
