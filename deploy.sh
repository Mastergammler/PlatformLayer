#!/bin/bash

#TODO: proper version management
VERSION="0.1.2"
OUTPUT_DIR="release"
#relative to output dir
ZIP_DIR="../publish"
EXEC_NAME=BeatNinja.exe
ZIP_NAME="BeatNinja-v$VERSION.zip"

rm -r $OUTPUT_DIR
mkdir $OUTPUT_DIR
mkdir $OUTPUT_DIR/logs

if [ ! -d $ZIP_DIR ]; then
    mkdir $ZIP_DIR
fi

./build.sh $OUTPUT_DIR $EXEC_NAME -o

if [ $? -eq 0 ]; then
    cp -r res $OUTPUT_DIR
    cp -r configs $OUTPUT_DIR

    cd $OUTPUT_DIR
    
    # pack the contents without a parent directory
    # expecting out dir to be a single dir path!!!
    zip -r $ZIP_DIR/$ZIP_NAME *
fi
