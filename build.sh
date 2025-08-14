#!/bin/bash

# $WIN_SDK $WIN_LIB $MSVC env variables have to be set to the windows sdk libraries of the version used
# eg.: ../WindowsKits/10/Include/10.0.026100.0 | ..Lib/10.0.26100.0 | ..MSVC/14.40.33807

# compiling external c header files etc
C_FILES="build/compile_extern.c"
C_FILES_OUTPUT=".obj/compile_extern.o"

SOURCE_FILE="build/compile.cpp"
PRECOMPILE_FILE="src/imports.h"

DEFAULT_BUILD_DIR=".build"
DEFAULT_EXEC_NAME="platform-demo.exe"
PRECOMPILE_OUTPUT=".obj/imports.h.pch"

if [[ "$1" =~ ^- ]] || [ -z "$1" ]; then
    BUILD_DIR=$DEFAULT_BUILD_DIR
else 
    BUILD_DIR=$1
fi

if [[ "$2" =~ ^- ]] || [ -z "$2" ]; then
    EXEC_NAME=$DEFAULT_EXEC_NAME
else
    EXEC_NAME=$2
fi

OUTPUT_FILE=$BUILD_DIR/$EXEC_NAME

# v for verbose
if [[ "$*" == *"-v"* ]]; then
    DEBUG="-v"
fi

# p for profile
if [[ "$*" == *"-p"* ]]; then
    PROFILE="-ftime-trace -c"
fi

if [[ "$*" == *"-o"* ]]; then
    OPTIMIZATION="-O3"
fi

# isystem supresses all the warning in the header files
# which windows seems to have a ton of
# NOTE: the windows sdk has a bunch of lower/upper case naming issues in it, that need to be fixed
# - an easy way to do that, is just to create a symbolic link to those "misspelled" files
SDK_INCLUDES="-isystem $WIN_SDK/um -isystem $WIN_SDK/shared -isystem $WIN_SDK/ucrt -isystem $WIN_SDK/winrt -isystem $WIN_SDK/cppwinrt"
MSVC_INCLUDES="-isystem $MSVC/include -isystem $MSVC/atlmfc/include"

# libs have the same case insensitive search problem
# It seems a better approach would be setting up a virtual filesystem, that all works out of the box
# Else i need to adjust it every single time
SDK_LIBS="-L$WIN_LIB/crt/x64 -L$WIN_LIB/um/x64 -L$WIN_LIB/ucrt/x64 -L$MSVC/lib/x64 -L$MSVC/atlmfc/lib/x64"

# including std 20 adds 600ms extra compile time for compiling with pre-compiled headers (1250ms vs 700ms)
# and for precompiling the headers its 2s (3500ms vs 1500ms)
# But 14 is the minimum required one so that the windows headers still work
# Even thou, it seems, this adds a whole bunch of compile time (maybe this is the major thing that introduces it?)
CPP_STD="-std=c++14"
# LLVM first that clang intrinsics will be used!
# MSVC handles intrinsics differently than clang, so clang 
# is not able to use MSVC intrinsics headers
INCLUDES="-isystem $LLVM/include $MSVC_INCLUDES $SDK_INCLUDES"
LIB_PATHS="$SDK_LIBS $MSVC_LIBS"
INCLUDE_PRECOMPILED="-include-pch $PRECOMPILE_OUTPUT"
TARGET="-target x86_64-pc-win32 -fuse-ld=lld"
#LINKED_LIBS="-lgdi32 -luser32 -lole32 -lshell32 -lwinmm -lmfreadwrite -lmfplat -lmfuuid -lxaudio2"
LINKED_LIBS="-lgdi32 -luser32 -lshell32 -lwinmm"
# if any library uses SIMD we need to enable intrinsics (and include the LLVM intrinsic headers, see above)
# we also need to enable mmx since the target architecture is x86_64-win32 (for legacy support) 
INTRINSICS="-mmmx -msse2"
OPTIONS="$DEBUG -Wall -Wpedantic -Wno-c99-extensions -Wno-c++17-extensions $PROFILE -DDEBUG $INTRINSICS" 

# -D_AMD64_ or -DX86_ = 1 is required for some windows headers
MSVC_DEFAULTS="$TARGET $INCLUDES -D_AMD64_=1" #-D_MSC_VER=1933"

PRE_COMPILE_START=$(date +%s%N)

# h for headers
if [[ "$*" == *"-h"* || "$*" == *"-o"* ]]; then
    #clang -c $C_FILES -o $C_FILES_OUTPUT $MSVC_DEFAULTS -w $OPTIMIZATION $INTRINSICS 
    clang++ -x c++-header $PRECOMPILE_FILE -o $PRECOMPILE_OUTPUT $MSVC_DEFAULTS $CPP_STD $DEBUG $OPTIMIZATION $INTRINSICS 

    if [ $? -ne 0 ]; then
        exit 1
    fi
fi

PRE_COMPILE_END=$(date +%s%N)
START_TIME=$(date +%s%N)

#TODO: cleaner setup for this

#clang++ $SOURCE_FILE $C_FILES_OUTPUT -o $OUTPUT_FILE $INCLUDE_PRECOMPILED $MSVC_DEFAULTS $CPP_STD $LIB_PATHS $LINKED_LIBS $OPTIONS $OPTIMIZATION
clang++ $SOURCE_FILE -o $OUTPUT_FILE $INCLUDE_PRECOMPILED $MSVC_DEFAULTS $CPP_STD $LIB_PATHS $LINKED_LIBS $OPTIONS $OPTIMIZATION
COMPILE_STATUS=$?

END_TIME=$(date +%s%N)

# NOTE: this is not necessary for running, because we're starting the game from the root dir
# and then the root dir will be the executing directory for the game on linux
#cp -r ./res $BUILD_DIR/res

NANO_TO_MS=1000000
COMPILE_TIME=$(((END_TIME - START_TIME) / $NANO_TO_MS))
PRE_COMPILE_TIME=$(((PRE_COMPILE_END - PRE_COMPILE_START)/ $NANO_TO_MS))

# Check for errors
if [ $COMPILE_STATUS -eq 0 ]; then
    echo "Compilation successful." 
fi

PC_CHARS=${#PRE_COMPILE_TIME}
C_CHARS=${#COMPILE_TIME}
if (( $C_CHARS > $PC_CHARS )); then
    MAX_CHARS=$C_CHARS
else
    MAX_CHARS=$PC_CHARS
fi

# since seq / printf doesn't support 0, we just increment it by 1
# and then truncate one later
PAD_PC=$(printf "%.s " $(seq 1 $(($MAX_CHARS-$PC_CHARS+1))))
PAD_C=$(printf "%.s " $(seq 1 $(($MAX_CHARS-$C_CHARS+1))))

echo  "| Pre-Compile :: ${PAD_PC:1}$PRE_COMPILE_TIME ms"
echo  "| Compile     :: ${PAD_C:1}$COMPILE_TIME ms"

exit $COMPILE_STATUS
