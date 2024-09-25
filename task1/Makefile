#-------------------------------------------------------------------

# To enable support for dynamic instruction counting,
# the following tools must be used. Please do not modify. 
CC=/usr/bin/clang-16
CXX=/usr/bin/clang++-16
LD=/usr/bin/ld.lld-16

INCLUDES=$(TECHARENA24_TASK1_DIR)/common/install/include
COMMON_INCLUDES=$(TECHARENA24_TASK1_DIR)/common
SYSROOT=$(TECHARENA24_TASK1_DIR)/common/install
LIBS=$(TECHARENA24_TASK1_DIR)/common/install/lib
DYN_INSTR_COUNT_DIR=$(TECHARENA24_TASK1_DIR)/common/DynamicInstructionCounting

CXXFLAGS=--sysroot $(SYSROOT) -nostdlib -nostdinc++ -nostdlib++ \
	 -isystem $(INCLUDES)/c++/v1/ -isystem $(INCLUDES)/x86_64-pc-linux-musl/c++/v1 -isystem $(COMMON_INCLUDES) \
	 -fPIC \
	 -fuse-ld=$(LD) -Wno-unused-command-line-argument \
         -D _LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION

LDFLAGS=--sysroot $(SYSROOT) -nostdlib -nostdinc++ -nostdlib++ \
        -L $(LIBS) -Wl,--rpath,$(LIBS) -L $(LIBS)/x86_64-pc-linux-musl -Wl,-rpath,$(LIBS)/x86_64-pc-linux-musl \
	-L $(DYN_INSTR_COUNT_DIR) -Wl,--rpath,$(DYN_INSTR_COUNT_DIR) \
        -latomic -lc++ -lc -lc++abi -lunwind -lDynamicInstructionCounting_API -lboost_program_options \
	-fuse-ld=$(LD) -Wno-unused-command-line-argument

# To enable support dynamic instruction counting,
# the following Clang command-line options must be added.
# Please do not modify. 
CXXFLAGS += -fpass-plugin=$(DYN_INSTR_COUNT_DIR)/libDynamicInstructionCounting.so 

#-------------------------------------------------------------------

# Main makefile logic to prepare libTask1PredictionAlgorithm.so. 
# This part can be modified if needed. 

SRC_FILES := $(wildcard ./*.cpp)
OBJ_FILES := $(patsubst ./%.cpp,./%.o,$(SRC_FILES))

all: task1

./PredictionAlgorithm/libTask1PredictionAlgorithm.so:
	$(MAKE) -C PredictionAlgorithm

task1: $(OBJ_FILES) ./PredictionAlgorithm/libTask1PredictionAlgorithm.so 
	$(CC) $(LDFLAGS) $(LLLDFLAGS) $^ $(LIBS)/crt1.o -o task1

./%.o: ./%.cpp
	$(CC) -c $(CXXFLAGS) $(LLCXXFLAGS) -o $@ $<

clean:
	$(MAKE) -C PredictionAlgorithm clean
	rm -rf *.o task1 
