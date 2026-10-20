#ifndef ECL_TESTS_HEADER_ONLY
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#endif

#define BOOST_TEST_MODULE ecl_test

#ifdef ECL_TESTS_HEADER_ONLY
#include <boost/test/included/unit_test.hpp>
#endif

#define ECL_WITH_STD_STRING

#include "test_singleton.hpp"
#include "test_cb.hpp"
#include "test_str_const.hpp"
#include "test_scope_guard.hpp"
#include "test_fsm.hpp"
#include "test_bitfield.hpp"
#include "test_json.hpp"
#include "test_tree.hpp"
#include "test_map.hpp"
#include "test_stream.hpp"
#include "test_command_processor.hpp"
