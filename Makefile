ARM_LD_FLAGS = --specs=rdimon.specs
OPTIMIZATION = -O0
DEBUG = -g2

WARNINGS = -Wall -Wextra -pedantic -Wswitch -Wnon-virtual-dtor -Wshadow -Weffc++

SECURE = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -ftrapv -Wformat-security -fPIE -Wl,-z,relro,-z,now -fstack-protector -ggdb

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
STATIC_MAP = static_map
TREE = tree
CMD = command_processor
MAP = map
JSON = json
WEB = web

INCLUDE_DIR = ./include
EXAMPLES_DIR = ./examples
TESTS_DIR = ./tests
BIN_DIR = ./bin
DOC_DIR = ./doc

HTTP_PARSER_DIR = ./thirdparty/http-parser

TESTS_BIN = tests

EXAMPLE_PREFIX = example
GCOV_PREFIX = gcov

WEB_DEF_PAGES_DIR=./web_def_pages
WEB_RES_SRC_DIR = ./examples/web_resources_src
WEB_RES_GEN_DIR = ./examples/web_resources
WEB_GEN_SOURCES = $(WEB_RES_GEN_DIR)/400_html.cpp              \
				  $(WEB_RES_GEN_DIR)/403_html.cpp              \
				  $(WEB_RES_GEN_DIR)/404_html.cpp              \
				  $(WEB_RES_GEN_DIR)/500_html.cpp              \
				  $(WEB_RES_GEN_DIR)/favicon_png.cpp           \
				  $(WEB_RES_GEN_DIR)/icon_png.cpp              \
				  $(WEB_RES_GEN_DIR)/index_html.cpp            \
				  $(WEB_RES_GEN_DIR)/authorized_index_html.cpp \
				  $(WEB_RES_GEN_DIR)/jquery_js.cpp             \
				  $(WEB_RES_GEN_DIR)/style_css.cpp

ifndef GCOV
	GCOV=gcov
endif

all: examples tests

out_dir:
	@mkdir -p $(BIN_DIR)

examples: $(EXAMPLE_PREFIX)_$(FSM) $(EXAMPLE_PREFIX)_$(SG) $(EXAMPLE_PREFIX)_$(CB) $(EXAMPLE_PREFIX)_$(BF) $(EXAMPLE_PREFIX)_$(SINGLETON) $(EXAMPLE_PREFIX)_$(STREAM) $(EXAMPLE_PREFIX)_$(JSON) $(EXAMPLE_PREFIX)_$(STR_CONST) $(EXAMPLE_PREFIX)_$(CMD) $(EXAMPLE_PREFIX)_$(TREE) $(EXAMPLE_PREFIX)_$(STATIC_MAP)

gen_web_res:
	@mkdir -p $(WEB_RES_GEN_DIR)
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/400.html            $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/403.html            $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/404.html            $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_DEF_PAGES_DIR)/500.html            $(WEB_RES_GEN_DIR)/
	./res_gen.sh $(WEB_RES_SRC_DIR)/index.html            $(WEB_RES_GEN_DIR)/ -c
	./res_gen.sh $(WEB_RES_SRC_DIR)/authorized_index.html $(WEB_RES_GEN_DIR)/ -c
	./res_gen.sh $(WEB_RES_SRC_DIR)/style.css             $(WEB_RES_GEN_DIR)/ -c
	./res_gen.sh $(WEB_RES_SRC_DIR)/icon.png              $(WEB_RES_GEN_DIR)/ -c
	./res_gen.sh $(WEB_RES_SRC_DIR)/favicon.png           $(WEB_RES_GEN_DIR)/ -c
	./res_gen.sh $(WEB_RES_SRC_DIR)/jquery.js             $(WEB_RES_GEN_DIR)/ -c

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

$(EXAMPLE_PREFIX)_$(WEB): out_dir gen_web_res
	$(CXX) $(FLAGS) -I$(INCLUDE_DIR) -I$(HTTP_PARSER_DIR) -Wl,-Map=$(BIN_DIR)/$(WEB)_$(CXX).map $(EXAMPLES_DIR)/$(EXAMPLE_PREFIX)_$(WEB).cpp $(WEB_GEN_SOURCES) $(HTTP_PARSER_DIR)/http_parser.c -o $(BIN_DIR)/$(WEB)_$(CXX)

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
	$(RM) -r $(WEB_RES_GEN_DIR)
