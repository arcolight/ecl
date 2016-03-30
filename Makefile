ARM_LD_FLAGS = --specs=rdimon.specs
OPTIMIZATION = -O2
DEBUG = -g0

GCC_WARNINGS = -Wall -Wextra -pedantic -Wswitch -Wnon-virtual-dtor
CLANG_WARNINGS = -Weverything -Wno-padded -Wno-c++98-compat -Wno-covered-switch-default -Wno-exit-time-destructors

WARNINGS = -Wall -Wextra -pedantic -Wswitch -Wnon-virtual-dtor

SECURE = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -ftrapv -Wformat-security -fPIE -Wl,-z,relro,-z,now

CXXFLAGS = -std=c++11 -fPIC -fno-rtti -fno-exceptions -fvisibility=hidden $(SECURE) $(OPTIMIZATION) $(DEBUG)

GCC_FLAGS = $(CXXFLAGS) $(GCC_WARNINGS) -fstack-protector -fstack-check -fbounds-check -ggdb
CLANG_FLAGS = $(CXXFLAGS) $(CLANG_WARNINGS)

FLAGS = $(CXXFLAGS) $(WARNINGS) -coverage $(ADD_FLAGS) $(ADD_WARN)

DOXYGEN_CONFIG = doxygen.config

FSM = fsm
SG = scope_guard
CB = cb
BF = bitfield
SINGLETON = singleton
STREAM = stream
STR_CONST = str_const
STATIC_MAP = static_map
TREE = tree
CMD = command_processor
STATIC_MAP = static_map
JSON = json

INCLUDE_DIR = ./include
TESTS_DIR = ./tests
BIN_DIR = ./bin
DOC_DIR = ./doc

GCOV = gcov

all: tests

out_dir:
	@mkdir -p $(BIN_DIR)

tests: test_$(FSM) test_$(SG) test_$(CB) test_$(BF) test_$(SINGLETON) test_$(STREAM) test_$(JSON) test_$(STR_CONST) test_$(CMD) test_$(TREE) test_$(STATIC_MAP)

test_$(FSM): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(FSM)_$(CXX).map $(TESTS_DIR)/test_$(FSM).cpp -o $(BIN_DIR)/$(FSM)_$(CXX)

test_$(SG): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SG)_$(CXX).map $(TESTS_DIR)/test_$(SG).cpp -o $(BIN_DIR)/$(SG)_$(CXX)

test_$(CB): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CB)_$(CXX).map $(TESTS_DIR)/test_$(CB).cpp -o $(BIN_DIR)/$(CB)_$(CXX)

test_$(BF): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(BF)_$(CXX).map $(TESTS_DIR)/test_$(BF).cpp -o $(BIN_DIR)/$(BF)_$(CXX)

test_$(SINGLETON): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(SINGLETON)_$(CXX).map $(TESTS_DIR)/test_$(SINGLETON).cpp -o $(BIN_DIR)/$(SINGLETON)_$(CXX)

test_$(STREAM): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STREAM)_$(CXX).map $(TESTS_DIR)/test_$(STREAM).cpp -o $(BIN_DIR)/$(STREAM)_$(CXX)

test_$(JSON): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(JSON)_$(CXX).map $(TESTS_DIR)/test_$(JSON).cpp -o $(BIN_DIR)/$(JSON)_$(CXX)

test_$(STR_CONST): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STR_CONST)_$(CXX).map $(TESTS_DIR)/test_$(STR_CONST).cpp -o $(BIN_DIR)/$(STR_CONST)_$(CXX)

test_$(STATIC_MAP): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(STATIC_MAP)_$(CXX).map $(TESTS_DIR)/test_$(STATIC_MAP).cpp -o $(BIN_DIR)/$(STATIC_MAP)_$(CXX)

test_$(TREE): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(TREE)_$(CXX).map $(TESTS_DIR)/test_$(TREE).cpp -o $(BIN_DIR)/$(TREE)_$(CXX)

test_$(CMD): out_dir
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -Wl,-Map=$(BIN_DIR)/$(CMD)_$(CXX).map $(TESTS_DIR)/test_$(CMD).cpp -o $(BIN_DIR)/$(CMD)_$(CXX)

gcov_$(FSM): test_$(FSM)
	$(BIN_DIR)/$(FSM)_$(CXX)
	cp test_$(FSM).gcno $(TESTS_DIR)/
	cp test_$(FSM).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(FSM).cpp
	rm $(TESTS_DIR)/test_$(FSM).gcno
	rm $(TESTS_DIR)/test_$(FSM).gcda

gcov_$(SG): test_$(SG)
	$(BIN_DIR)/$(SG)_$(CXX)
	cp test_$(SG).gcno $(TESTS_DIR)/
	cp test_$(SG).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(SG).cpp
	rm $(TESTS_DIR)/test_$(SG).gcno
	rm $(TESTS_DIR)/test_$(SG).gcda

gcov_$(CB): test_$(CB)
	$(BIN_DIR)/$(CB)_$(CXX)
	cp test_$(CB).gcno $(TESTS_DIR)/
	cp test_$(CB).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(CB).cpp
	rm $(TESTS_DIR)/test_$(CB).gcno
	rm $(TESTS_DIR)/test_$(CB).gcda

gcov_$(BF): test_$(BF)
	$(BIN_DIR)/$(BF)_$(CXX)
	cp test_$(BF).gcno $(TESTS_DIR)/
	cp test_$(BF).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(BF).cpp
	rm $(TESTS_DIR)/test_$(BF).gcno
	rm $(TESTS_DIR)/test_$(BF).gcda

gcov_$(SINGLETON): test_$(SINGLETON)
	$(BIN_DIR)/$(SINGLETON)_$(CXX)
	cp test_$(SINGLETON).gcno $(TESTS_DIR)/
	cp test_$(SINGLETON).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(SINGLETON).cpp
	rm $(TESTS_DIR)/test_$(SINGLETON).gcno
	rm $(TESTS_DIR)/test_$(SINGLETON).gcda

gcov_$(STREAM): test_$(STREAM)
	$(BIN_DIR)/$(STREAM)_$(CXX)
	cp test_$(STREAM).gcno $(TESTS_DIR)/
	cp test_$(STREAM).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(STREAM).cpp
	rm $(TESTS_DIR)/test_$(STREAM).gcno
	rm $(TESTS_DIR)/test_$(STREAM).gcda

gcov_$(JSON): test_$(JSON)
	$(BIN_DIR)/$(JSON)_$(CXX)
	cp test_$(JSON).gcno $(TESTS_DIR)/
	cp test_$(JSON).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(JSON).cpp
	rm $(TESTS_DIR)/test_$(JSON).gcno
	rm $(TESTS_DIR)/test_$(JSON).gcda

gcov_$(STR_CONST): test_$(STR_CONST)
	$(BIN_DIR)/$(STR_CONST)_$(CXX)
	cp test_$(STR_CONST).gcno $(TESTS_DIR)/
	cp test_$(STR_CONST).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(STR_CONST).cpp
	rm $(TESTS_DIR)/test_$(STR_CONST).gcno
	rm $(TESTS_DIR)/test_$(STR_CONST).gcda

gcov_$(STATIC_MAP): test_$(STATIC_MAP)
	$(BIN_DIR)/$(STATIC_MAP)_$(CXX)
	cp test_$(STATIC_MAP).gcno $(TESTS_DIR)/
	cp test_$(STATIC_MAP).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(STATIC_MAP).cpp
	rm $(TESTS_DIR)/test_$(STATIC_MAP).gcno
	rm $(TESTS_DIR)/test_$(STATIC_MAP).gcda

gcov_$(TREE): test_$(TREE)
	$(BIN_DIR)/$(TREE)_$(CXX)
	cp test_$(TREE).gcno $(TESTS_DIR)/
	cp test_$(TREE).gcda $(TESTS_DIR)/
	$(GCOV) $(TESTS_DIR)/test_$(TREE).cpp
	rm $(TESTS_DIR)/test_$(TREE).gcno
	rm $(TESTS_DIR)/test_$(TREE).gcda

gcov_all: gcov_$(FSM) gcov_$(SG) gcov_$(CB) gcov_$(BF) gcov_$(SINGLETON) gcov_$(STREAM) gcov_$(JSON) gcov_$(STR_CONST) gcov_$(STATIC_MAP) gcov_$(TREE)

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
