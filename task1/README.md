1. Source source.sh from task1 directory to setup environment variables
source source.sh

2. Run ./scripts/setup_infrastructure.sh from task1 directory. This script will:
  a. unzip files containing routes for experiments
  b. install a docker image with toolchaini that should be used to build a solution

3. A solution should be implemented as ./task1/PredictionAlgorithm/libTask1PredictionAlgorithm.so library 

4. To build a solution using the custom toolchain, run 
./scripts/build.sh

5. To evaluate a solution using the custom toolchain, run 
./scripts/evaluate.sh -r routes/route.txt
