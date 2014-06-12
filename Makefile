GCC_SPECIFIC = -fstack-check -fbounds-check
ARM_LD_FLAGS = --specs=rdimon.specs -lrdimon
OPTIMIZATION = -O3
DEBUG = -g0
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC -fno-rtti -fno-exceptions $(OPTIMIZATION) $(DEBUG)

FSM = fsm
CB = cb

INCLUDE_DIR = ./include/
TESTS_DIR = ./tests/
BIN_DIR = ./bin/

GCC_TGT = _gcc_x86
CLANG_TGT = _clang_x86
GCC_ARM_TGT = _gcc_arm

all: gcc clang arm

gcc:
	@mkdir -p $(BIN_DIR)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_TGT)

clang:
	@mkdir -p $(BIN_DIR)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(CLANG_TGT)

arm:
	@mkdir -p $(BIN_DIR)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_ARM_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_ARM_TGT)

clean:
	$(RM) -r $(BIN_DIR)
