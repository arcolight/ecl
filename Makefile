GCC_SPECIFIC = -fstack-check -fbounds-check -ggdb
ARM_LD_FLAGS = --specs=rdimon.specs -lrdimon
OPTIMIZATION = -O0
DEBUG = -g3
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC -fno-rtti -fno-exceptions $(OPTIMIZATION) $(DEBUG)

FSM = fsm
CB = cb
BF = bitfield
SINGLETON = singleton
LOG = log
STR_CONST = str_const
WEB = web
STATIC_MAP = static_map

INCLUDE_DIR = ./include
TESTS_DIR = ./tests
BIN_DIR = ./bin

GCC_TGT = _gcc_x86
CLANG_TGT = _clang_x86
GCC_ARM_TGT = _gcc_arm

all: gcc_x86 clang_x86 gcc_arm_noeabi

gcc_x86:
	@mkdir -p $(BIN_DIR)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(WEB).cpp -o $(BIN_DIR)/test_$(WEB)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STATIC_MAP).cpp -o $(BIN_DIR)/test_$(STATIC_MAP)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_TGT)

clang_x86:
	@mkdir -p $(BIN_DIR)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STATIC_MAP).cpp -o $(BIN_DIR)/test_$(STATIC_MAP)$(CLANG_TGT)
#	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(CLANG_TGT)

gcc_arm_noeabi:
	@mkdir -p $(BIN_DIR)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_ARM_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_ARM_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_ARM_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_ARM_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(GCC_ARM_TGT)
#	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_ARM_TGT)

gcc_arm_elf_eabi:
	@mkdir -p $(BIN_DIR)
	arm-elf-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_ARM_TGT)
	arm-elf-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_ARM_TGT)
	arm-elf-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_ARM_TGT)
	arm-elf-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_ARM_TGT)
	arm-elf-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(GCC_ARM_TGT)
#	arm-elf-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_ARM_TGT)

clean:
	$(RM) -r $(BIN_DIR)
