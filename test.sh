
#!/bin/bash

SOURCE_FILE="build/test.cpp"
OUTPUT_DIR=".build"
export OUTPUT_FILE="$OUTPUT_DIR/test"
export WIN32=0

OPTIONS="-std=c++14 -Wall -Wno-c++11-extensions"

T_START=$(date +%s%N)

clang++ $SOURCE_FILE -o $OUTPUT_FILE $OPTIONS
export COMPILE_STATUS=$?

T_END=$(date +%s%N)

CST_NANO_TO_MS=1000000
T_ELAPSED=$(((T_END - T_START)/ $CST_NANO_TO_MS))

if [ $COMPILE_STATUS -eq 0 ]; then

    echo "************************"
    echo "   Compile :: $T_ELAPSED ms "
    echo "************************"

    $OUTPUT_FILE
fi

