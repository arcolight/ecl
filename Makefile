GCC_SPECIFIC = -fstack-check -fbounds-check -ggdb
ARM_LD_FLAGS = --specs=rdimon.specs -lrdimon
OPTIMIZATION = -O2
DEBUG = -g3
SECURE = -D_FORTIFY_SOURCE=2 -ftrapv -Wformat-security -fPIE -Wl,-z,relro,-z,now
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Wswitch -fPIC -fno-rtti -fno-exceptions $(SECURE) $(OPTIMIZATION) $(DEBUG)

FSM = fsm
SG = scope_guard
CB = cb
BF = bitfield
SINGLETON = singleton
STREAM = stream
STR_CONST = str_const
WEB = web
STATIC_MAP = static_map
JSON = json
CE_MD5 = constexpr_md5

INCLUDE_DIR = ./include
TESTS_DIR = ./tests
BIN_DIR = ./bin
WEB_RES_SRC_DIR = ./tests/web_resources_src
WEB_RES_GEN_DIR = ./tests/web_resources

GCC_TGT = _gcc_x86
CLANG_TGT = _clang_x86
GCC_ARM_TGT = _gcc_arm
GCC_ARM_NONE_TGT = _gcc_arm_none

all: gcc clang

gen_web_res:
	@mkdir -p $(WEB_RES_GEN_DIR)
	./res_gen.sh $(WEB_RES_SRC_DIR)/index.html  $(WEB_RES_GEN_DIR)/index.h
	./res_gen.sh $(WEB_RES_SRC_DIR)/style.css   $(WEB_RES_GEN_DIR)/style.h
	./res_gen.sh $(WEB_RES_SRC_DIR)/icon.png    $(WEB_RES_GEN_DIR)/icon.h
	./res_gen.sh $(WEB_RES_SRC_DIR)/favicon.png $(WEB_RES_GEN_DIR)/favicon.h
	./res_gen.sh $(WEB_RES_SRC_DIR)/jquery.js   $(WEB_RES_GEN_DIR)/jquery.h

gcc: gen_web_res
	@mkdir -p $(BIN_DIR)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SG).cpp -o $(BIN_DIR)/test_$(SG)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/test_$(STREAM)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/test_$(JSON)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(WEB).cpp -o $(BIN_DIR)/test_$(WEB)$(GCC_TGT)
#	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CE_MD5).cpp -o $(BIN_DIR)/test_$(CE_MD5)$(GCC_TGT)

clang: gen_web_res
	@mkdir -p $(BIN_DIR)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/test_$(STREAM)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/test_$(JSON)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(WEB).cpp -o $(BIN_DIR)/test_$(WEB)$(CLANG_TGT)
#	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CE_MD5).cpp -o $(BIN_DIR)/test_$(CE_MD5)$(CLANG_TGT)

gcc_arm_none_eabi:
	@mkdir -p $(BIN_DIR)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/test_$(STREAM)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/test_$(JSON)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_ARM_NONE_TGT)
#	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(WEB).cpp -o $(BIN_DIR)/test_$(WEB)$(GCC_ARM_NONE_TGT)
#	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CE_MD5).cpp -o $(BIN_DIR)/test_$(CE_MD5)$(GCC_ARM_NONE_TGT)

clean:
	$(RM) -r $(BIN_DIR)
	$(RM) -r $(WEB_RES_GEN_DIR)
