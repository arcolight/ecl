ARM_LD_FLAGS = --specs=rdimon.specs
OPTIMIZATION = -O0
DEBUG = -g2

WARNINGS = -Wall -Wextra -pedantic -Wswitch -Wnon-virtual-dtor -Wshadow -Weffc++

SECURE = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -ftrapv -Wformat-security -fPIE -Wl,-z,relro,-z,now -ggdb

CXXFLAGS = -std=c++11 -fPIC -fno-rtti -fno-exceptions -fvisibility=hidden $(SECURE) $(OPTIMIZATION) $(DEBUG)
TEST_CXX_FLAGS = -std=c++11 -fPIC -fvisibility=hidden $(SECURE) $(OPTIMIZATION) $(DEBUG)

FLAGS = $(CXXFLAGS) $(WARNINGS) $(ADD_FLAGS) $(ADD_WARN)
TESTS_FLAGS = $(TEST_CXX_FLAGS) $(WARNINGS) $(ADD_FLAGS) $(ADD_WARN) -coverage

DOXYGEN_CONFIG = doxygen.config

FSM = fsm
SG = scope_guard
CB = cb
BF = bitfield
SINGLETON = singleton
STREAM = stream
STR_CONST = str_const
TREE = tree
CMD = command_processor
MAP = map
JSON = json

INCLUDE_DIR = ./include
EXAMPLES_DIR = ./examples
TESTS_DIR = ./tests
BIN_DIR = ./bin
DOC_DIR = ./doc

TESTS_BIN = tests

EXAMPLE_PREFIX = example
GCOV_PREFIX = gcov

ifndef GCOV
	GCOV=gcov
endif

all: examples tests

out_dir:
	@mkdir -p $(BIN_DIR)

examples: $(EXAMPLE_PREFIX)_$(FSM) $(EXAMPLE_PREFIX)_$(SG) $(EXAMPLE_PREFIX)_$(CB) $(EXAMPLE_PREFIX)_$(BF) $(EXAMPLE_PREFIX)_$(SINGLETON) $(EXAMPLE_PREFIX)_$(STREAM) $(EXAMPLE_PREFIX)_$(JSON) $(EXAMPLE_PREFIX)_$(STR_CONST) $(EXAMPLE_PREFIX)_$(CMD) $(EXAMPLE_PREFIX)_$(TREE) $(EXAMPLE_PREFIX)_$(MAP)

$(EXAMPLE_PREFIX)_$(FSM): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(FSM)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(FSM).cpp -o $(BIN_DIR)/$(FSM)_$(CXX)

$(EXAMPLE_PREFIX)_$(SG): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SG)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(SG).cpp -o $(BIN_DIR)/$(SG)_$(CXX)

$(EXAMPLE_PREFIX)_$(CB): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CB)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(CB).cpp -o $(BIN_DIR)/$(CB)_$(CXX)

$(EXAMPLE_PREFIX)_$(BF): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(BF)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(BF).cpp -o $(BIN_DIR)/$(BF)_$(CXX)

$(EXAMPLE_PREFIX)_$(SINGLETON): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SINGLETON)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(SINGLETON).cpp -o $(BIN_DIR)/$(SINGLETON)_$(CXX)

$(EXAMPLE_PREFIX)_$(STREAM): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STREAM)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(STREAM).cpp -o $(BIN_DIR)/$(STREAM)_$(CXX)

$(EXAMPLE_PREFIX)_$(JSON): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(JSON)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(JSON).cpp -o $(BIN_DIR)/$(JSON)_$(CXX)

$(EXAMPLE_PREFIX)_$(STR_CONST): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STR_CONST)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(STR_CONST).cpp -o $(BIN_DIR)/$(STR_CONST)_$(CXX)

$(EXAMPLE_PREFIX)_$(MAP): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(MAP)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(MAP).cpp -o $(BIN_DIR)/$(MAP)_$(CXX)

$(EXAMPLE_PREFIX)_$(TREE): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(TREE)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(TREE).cpp -o $(BIN_DIR)/$(TREE)_$(CXX)

$(EXAMPLE_PREFIX)_$(CMD): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CMD)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(CMD).cpp -o $(BIN_DIR)/$(CMD)_$(CXX)

tests: out_dir clean_gcov
	$(CXX) $(TESTS_FLAGS) -I$(INCLUDE_DIR) $(TESTS_DIR)/$(TESTS_BIN).cpp -lboost_unit_test_framework -o $(BIN_DIR)/$(TESTS_BIN)_$(CXX)

$(GCOV_PREFIX)_$(TESTS_BIN): $(TESTS_BIN)
	$(BIN_DIR)/$(TESTS_BIN)_$(CXX)
	cp $(TESTS_BIN).gcno $(TESTS_DIR)/
	cp $(TESTS_BIN).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/$(TESTS_BIN).cpp
	rm $(TESTS_DIR)/$(TESTS_BIN).gcno
	rm $(TESTS_DIR)/$(TESTS_BIN).gcda

$(GCOV_PREFIX)_all: $(GCOV_PREFIX)_$(TESTS_BIN)

doxygen:
	doxygen $(DOXYGEN_CONFIG)

clean_gcov:
	$(RM) *.gcno
	$(RM) *.gcda
	$(RM) *.gcov

clean_doc:
	$(RM) -r $(DOC_DIR)

clean: clean_doc clean_gcov
	$(RM) -r $(BIN_DIR)
