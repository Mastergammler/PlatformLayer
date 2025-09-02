#!/bin/bash

OUTPUT_DIR=".build"
EXEC_NAME=platform-demo.exe

export WINEDEBUG=+asio
#,+loaddll,+dll,+module
export WINEPREFIX=~/.gamedev
#export WINEPREFIX=~/.gd-noasio
export WINEASIO_NUMBER_INPUTS=0
export WINEASIO_NUMBER_OUTPUTS=4
export WINEASIO_FIXED_BUFFERSIZE=0
export WINEASIO_PREFERRED_BUFFERSIZE=256

./build.sh $OUTPUT_DIR $EXEC_NAME

if [ $? -eq 0 ]; then
    echo -e "\n---- POWERED BY WINE ----"

    # NOTE: wine searches in the directory relative to the executable!
    # not the current directory from which it was called
    # this is different to how the game itself searches for resources from the directory it was called
    wine-dev $OUTPUT_DIR/$EXEC_NAME debug
    #wine $OUTPUT_DIR/$EXEC_NAME debug
fi
