#/bin/bash 

PARAMETERS="$@"

# register ctrl+c handler
trap ctrl_c INT
function ctrl_c() {
    docker stop $CONTAINER_NAME
    echo "Ctrl+C registered. Stopping the evaluation docker container."
}

# check if TECHARENA24_TASK2_DIR is set
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

# check if docker image techarena24_toolchain:latest is installed
if [[ "$(docker images -q techarena24_toolchain:latest 2> /dev/null)" == "" ]]; then
    echo "The techarena24_toolchain docker image is not installed."
    echo "To install the techarena24_toolchain docker, please cd to task2 directory and"
    echo "run the command './scripts/setup_infrastructure.sh'."
    exit 1
fi

# check if task2 executable exists
if [[ ! -f "$TECHARENA24_TASK2_DIR/task2/task2" ]]; then
    echo "Can't find the executable file task2 to run for evaluation."
    echo "To create an executable file task2 please cd to task2 directory and"
    echo "run the command './scripts/build.sh'."
    exit 1
fi


# Run the toolchain docker and 
# launch the task2 executable file with given parameters.
CONTAINER_NAME=${USER}_${RANDOM}
docker \
    run \
    --rm \
    -v $TECHARENA24_TASK2_DIR:/project \
    -w /project \
    --init \
    --name $CONTAINER_NAME \
    techarena24_toolchain \
    /bin/bash -c  "cd /project/task2; ./task2 $PARAMETERS" & pid=$!

wait $pid
