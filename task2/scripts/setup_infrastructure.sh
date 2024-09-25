#!/bin/bash

# check that TECHARENA24_TASK2_DIR is set
if [[ -z "$TECHARENA24_TASK2_DIR" ]]; then 
  echo "Error: TECHARENA24_TASK2_DIR variable is not set. Please cd to task one directory and run:
  source source.sh
  ."
  exit 1
fi

# check if docker is installed and running 
if ! docker info > /dev/null 2>&1; then
  echo "This script uses docker, and it isn't running - please start docker and try again!"
  exit 1
fi

# check if gzip is installed
if [[ -z "$(which gzip)" ]]; then
    echo "This script requires gzip. Please install gzip and re-run this script."
    exit 1
fi

# uncompress the routes for task2 input
pushd $TECHARENA24_TASK2_DIR/task2/routes
echo "Unzipping testing and evaluation routes."
if [[ ! -z "$(ls -1 *.gz 2> /dev/null)" ]]; then
    for i in $(ls *.gz); do gzip -d $i; done 
fi
popd

if docker image inspect techarena24_toolchain &> /dev/null; then
    echo "Skipping installation of a docker image with toolchain since docker image with the name techarena24_toolchain already installed."
else

    # uncompress and load a docker image with the toolchain
    pushd $TECHARENA24_TASK2_DIR/toolchain/docker
    echo "Unpacking and installing a docker container with toolchain."
    cat techarena24_toolchain_docker.tar_gz_* | gzip -d --to-stdout  | docker load
    popd
fi
