#!/bin/bash

#This script will be used for evaluating a submission.


# check that TECHARENA24_TASK2_DIR is set
if [[ -z "$TECHARENA24_TASK2_DIR" ]]; then
    echo "The TECHARENA24_TASK2_DIR bash environment variable is not set."
    echo "To set the TECHARENA24_TASK2_DIR variable, please cd to task2 directory and"
    echo "run the command 'source source.sh'."
    exit 1
fi

# check if docker is installed and running
if ! docker info > /dev/null 2>&1; then
  echo "This script uses docker, and it isn't running - please start docker and try again!"
  exit 1
fi

# check that docker image techarena24_toolchain:latest is installed
if [[ "$(docker images -q techarena24_toolchain:latest 2> /dev/null)" == "" ]]; then
    echo "The techarena24_toolchain docker image is not installed."
    echo "To install the techarena24_toolchain docker, please cd to task2 directory and"
    echo "run the command './scripts/setup_infrastructure.sh'."
    exit 1
fi

echo "Entering $TECHARENA24_TASK2_DIR/task2 directory..."
cd $TECHARENA24_TASK2_DIR/task2

INPUT_TAR="$1"
echo "Testing a sample submission $INPUT_TAR"

echo "Trying to unpack the submission to $TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm directory"
if [ -d "$TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm" ]; then 
    echo "$TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm directory already exists."
    echo "Please remove remove it and rerun this script" 
    exit 1
#    mv $TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm $TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm.old
fi

mkdir -p $TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm && tar --strip-components 1 -xvf $INPUT_TAR -C $TECHARENA24_TASK2_DIR/task2/PredictionAlgorithm

echo "Building the submission..."
$TECHARENA24_TASK2_DIR/task2/scripts/build.sh > build.log
echo "Building has finished. Log is available in $(pwd)/build.log"

echo "Evaluating the submission..."
$TECHARENA24_TASK2_DIR/task2/scripts/evaluate.sh -r routes/route.txt > result.log 
echo "Evaluation has finished. Log is available in $(pwd)/result.log"
