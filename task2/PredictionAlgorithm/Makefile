#-------------------------------------------------------------------

# To enable support for dynamic instruction counting,
# the following tools must be used. Please do not modify. 
CC=/usr/bin/clang-16
CXX=/usr/bin/clang++-16
LD=/usr/bin/ld.lld-16

INCLUDES=$(TECHARENA24_TASK2_DIR)/common/install/include
COMMON_INCLUDES=$(TECHARENA24_TASK2_DIR)/common
SYSROOT=$(TECHARENA24_TASK2_DIR)/common/install
LIBS=$(TECHARENA24_TASK2_DIR)/common/install/lib
DYN_INSTR_COUNT_DIR=$(TECHARENA24_TASK2_DIR)/common/DynamicInstructionCounting

CXXFLAGS=--sysroot $(SYSROOT) -nostdlib -nostdinc++ -nostdlib++ \
	 -isystem $(INCLUDES)/c++/v1/ -isystem $(INCLUDES)/x86_64-pc-linux-musl/c++/v1 -isystem $(COMMON_INCLUDES) \
	 -fPIC \
	 -fuse-ld=$(LD) -Wno-unused-command-line-argument
	 

LDFLAGS=--sysroot $(SYSROOT) -nostdlib -nostdinc++ -nostdlib++ \
        -L $(LIBS) -Wl,--rpath,$(LIBS) -L $(LIBS)/x86_64-pc-linux-musl -Wl,-rpath,$(LIBS)/x86_64-pc-linux-musl \
		-L $(DYN_INSTR_COUNT_DIR) -Wl,--rpath,$(DYN_INSTR_COUNT_DIR) \
        -fPIC \
        -latomic -lc++ -lc -lc++abi -lunwind -lDynamicInstructionCounting_API \
		-fuse-ld=$(LD) -Wno-unused-command-line-argument

# To enable support dynamic instruction counting,
# the following Clang command-line options must be added.
# Please do not modify. 
CXXFLAGS += -fpass-plugin=$(DYN_INSTR_COUNT_DIR)/libDynamicInstructionCounting.so -Wno-unused-command-line-argument 

#-------------------------------------------------------------------

# Main makefile logic to prepare libTask1PredictionAlgorithm.so. 
# This part can be modified if needed. 

SRC_FILES := $(wildcard ./*.cpp)
OBJ_FILES := $(patsubst ./%.cpp,./%.o,$(SRC_FILES))

all: $(OBJ_FILES) 
	$(CC) $(LDFLAGS) $^ -shared -o libTask1PredictionAlgorithm.so

./%.o: ./%.cpp
	$(CC) -c $(CXXFLAGS) -shared -o $@ $<

clean:
	rm -rf *.o libTask1PredictionAlgorithm.so 
