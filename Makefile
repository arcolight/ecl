GCC_SPECIFIC = -fstack-check -fbounds-check -ggdb
ARM_LD_FLAGS = --specs=rdimon.specs
OPTIMIZATION = -O2
DEBUG = -g3
SECURE = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -ftrapv -Wformat-security -fPIE -Wl,-z,relro,-z,now
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Wswitch -Wnon-virtual-dtor -fPIC -fno-rtti -fno-exceptions -fvisibility=hidden $(SECURE) $(OPTIMIZATION) $(DEBUG)

FSM = fsm
SG = scope_guard
CB = cb
BF = bitfield
SINGLETON = singleton
STREAM = stream
STR_CONST = str_const
CMD = command_processor
WEB = web
STATIC_MAP = static_map
JSON = json
LOG = logger

INCLUDE_DIR = ./include
TESTS_DIR = ./tests
BIN_DIR = ./bin
WEB_DEF_PAGES_DIR=./web_def_pages
WEB_RES_SRC_DIR = ./tests/web_resources_src
WEB_RES_GEN_DIR = ./tests/web_resources
WEB_GEN_SOURCES = $(WEB_RES_GEN_DIR)/400_html.cpp $(WEB_RES_GEN_DIR)/403_html.cpp $(WEB_RES_GEN_DIR)/404_html.cpp $(WEB_RES_GEN_DIR)/500_html.cpp $(WEB_RES_GEN_DIR)/favicon_png.cpp $(WEB_RES_GEN_DIR)/icon_png.cpp $(WEB_RES_GEN_DIR)/index_html.cpp $(WEB_RES_GEN_DIR)/jquery_js.cpp $(WEB_RES_GEN_DIR)/style_css.cpp 

GCC_TGT = _gcc_x86
CLANG_TGT = _clang_x86
GCC_ARM_TGT = _gcc_arm
GCC_ARM_NONE_TGT = _gcc_arm_none_eabi

all: gcc clang

gen_web_res:
	@mkdir -p $(WEB_RES_GEN_DIR)
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/400.html  $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/403.html  $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/404.html  $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/500.html  $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_RES_SRC_DIR)/index.html  $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_RES_SRC_DIR)/style.css   $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_RES_SRC_DIR)/icon.png    $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_RES_SRC_DIR)/favicon.png $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_RES_SRC_DIR)/jquery.js   $(WEB_RES_GEN_DIR)/

gcc: gen_web_res
	@mkdir -p $(BIN_DIR)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(FSM)$(GCC_TGT).map $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SG)$(GCC_TGT).map $(TESTS_DIR)/test_$(SG).cpp -o $(BIN_DIR)/test_$(SG)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CB)$(GCC_TGT).map $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(BF)$(GCC_TGT).map $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SINGLETON)$(GCC_TGT).map $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STREAM)$(GCC_TGT).map $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/test_$(STREAM)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(JSON)$(GCC_TGT).map $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/test_$(JSON)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STR_CONST)$(GCC_TGT).map $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CMD)$(GCC_TGT).map $(TESTS_DIR)/test_$(CMD).cpp -o $(BIN_DIR)/test_$(CMD)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(LOG)$(GCC_TGT).map $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(GCC_TGT)
	g++ $(CXXFLAGS) $(GCC_SPECIFIC) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(WEB)$(GCC_TGT).map $(TESTS_DIR)/test_$(WEB).cpp $(WEB_GEN_SOURCES) -o $(BIN_DIR)/test_$(WEB)$(GCC_TGT)

clang: gen_web_res
	@mkdir -p $(BIN_DIR)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(FSM)$(CLANG_TGT).map $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SG)$(CLANG_TGT).map $(TESTS_DIR)/test_$(SG).cpp -o $(BIN_DIR)/test_$(SG)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CB)$(CLANG_TGT).map $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(BF)$(CLANG_TGT).map $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SINGLETON)$(CLANG_TGT).map $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STREAM)$(CLANG_TGT).map $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/test_$(STREAM)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(JSON)$(CLANG_TGT).map $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/test_$(JSON)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STR_CONST)$(CLANG_TGT).map $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CMD)$(CLANG_TGT).map $(TESTS_DIR)/test_$(CMD).cpp -o $(BIN_DIR)/test_$(CMD)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(LOG)$(CLANG_TGT).map $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(CLANG_TGT)
	clang++ $(CXXFLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(WEB)$(CLANG_TGT).map $(TESTS_DIR)/test_$(WEB).cpp $(WEB_GEN_SOURCES) -o $(BIN_DIR)/test_$(WEB)$(CLANG_TGT)

gcc_arm_none_eabi:
	@mkdir -p $(BIN_DIR)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(FSM)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/test_$(FSM)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(SG)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SG).cpp -o $(BIN_DIR)/test_$(SG)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(CB)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/test_$(CB)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(BF)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/test_$(BF)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(SINGLETON)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/test_$(SINGLETON)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(STREAM)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/test_$(STREAM)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(JSON)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/test_$(JSON)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(STR_CONST)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/test_$(STR_CONST)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(CMD)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(CMD).cpp -o $(BIN_DIR)/test_$(CMD)$(GCC_ARM_NONE_TGT)
	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(LOG)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(LOG).cpp -o $(BIN_DIR)/test_$(LOG)$(GCC_ARM_NONE_TGT)
#	arm-none-eabi-g++ $(CXXFLAGS) $(GCC_SPECIFIC) $(ARM_LD_FLAGS) -Wl,-Map=$(BIN_DIR)/$(WEB)$(GCC_ARM_NONE_TGT).map -I$(INCLUDE_DIR) $(TESTS_DIR)/test_$(WEB).cpp -o $(BIN_DIR)/test_$(WEB)$(GCC_ARM_NONE_TGT)

clean:
	$(RM) -r $(BIN_DIR)
	$(RM) -r $(WEB_RES_GEN_DIR)
