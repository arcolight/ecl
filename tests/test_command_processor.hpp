#ifndef ECL_TEST_COMMAND_PROCESSOR_HPP
#define ECL_TEST_COMMAND_PROCESSOR_HPP

#include <ecl/name_type.hpp>
#include <ecl/command.hpp>
#include <ecl/command_processor.hpp>

#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE( command_processor_suite )

ECL_DECL_NAME_TYPE(root)
ECL_DECL_NAME_TYPE(nested)
ECL_DECL_NAME_TYPE(echo)
ECL_DECL_NAME_TYPE(strict)
ECL_DECL_NAME_TYPE(deep)

struct command_echo : public ecl::command<echo, command_echo>
{
    bool init(const std::size_t argc, const char* const* const argv)
    {
        for(std::size_t i = 0; i < argc; ++i)
        {
            m_args.push_back(argv[i]);
        }
        return true;
    }

    std::vector<std::string> m_args {};
};

struct command_strict : public ecl::command<strict, command_strict>
{
    bool init(const std::size_t argc, const char* const* const argv)
    {
        if(1 != argc)
        {
            return false;
        }

        m_arg = argv[0];
        return true;
    }

    template<typename ST>
    static void show_help(ST& st, std::size_t indent, std::size_t = DEFAULT_INDENT_INCREMENT)
    {
        ecl::print_indent(st, indent);
        st << name() << " <arg>" << "\n\r";
    }

    std::string m_arg {};
};

struct command_deep : public ecl::command<deep, command_deep>
{
    bool init(const std::size_t argc, const char* const* const)
    {
        m_count = argc;
        return true;
    }

    std::size_t m_count { 0 };
};

using nested_processor_t = ecl::command_processor
<
      nested
    , command_deep
>;

using processor_t = ecl::command_processor
<
      root
    , command_echo
    , command_strict
    , nested_processor_t
>;

template<typename CMD>
struct recording_receiver : public ecl::receiver<CMD>
{
    recording_receiver()
        : received()
    {}

    void receive(CMD& c) override
    {
        received.push_back(c);
    }

    void clear()
    {
        received.clear();
    }

    std::vector<CMD> received;
};

static recording_receiver<command_echo>& echo_receiver()
{
    static recording_receiver<command_echo> r;
    return r;
}

static recording_receiver<command_echo>& echo_receiver_2()
{
    static recording_receiver<command_echo> r;
    return r;
}

static recording_receiver<command_strict>& strict_receiver()
{
    static recording_receiver<command_strict> r;
    return r;
}

static recording_receiver<command_deep>& deep_receiver()
{
    static recording_receiver<command_deep> r;
    return r;
}

struct command_processor_fixture
{
    command_processor_fixture()
        : out()
    {
        echo_receiver().clear();
        echo_receiver_2().clear();
        strict_receiver().clear();
        deep_receiver().clear();
    }

    template<std::size_t N>
    bool run(const char* (&argv)[N])
    {
        processor_t p;
        if(!p.init(N, argv))
        {
            return false;
        }
        return p.dispatch(out);
    }

    std::stringstream out;
};

BOOST_FIXTURE_TEST_CASE( dispatch_case, command_processor_fixture )
{
    const char* argv[] = { "echo", "a", "bc", "def" };
    BOOST_CHECK(run(argv));

    BOOST_REQUIRE_EQUAL(echo_receiver().received.size(), 1u);
    const std::vector<std::string> expected { "a", "bc", "def" };
    const auto& args = echo_receiver().received[0].m_args;
    BOOST_CHECK_EQUAL_COLLECTIONS(args.begin(), args.end(), expected.begin(), expected.end());

    BOOST_CHECK(strict_receiver().received.empty());
    BOOST_CHECK(deep_receiver().received.empty());
}

BOOST_FIXTURE_TEST_CASE( no_args_case, command_processor_fixture )
{
    const char* argv[] = { "echo" };
    BOOST_CHECK(run(argv));

    BOOST_REQUIRE_EQUAL(echo_receiver().received.size(), 1u);
    BOOST_CHECK(echo_receiver().received[0].m_args.empty());
}

BOOST_FIXTURE_TEST_CASE( multiple_receivers_case, command_processor_fixture )
{
    const char* argv[] = { "echo", "x" };
    BOOST_CHECK(run(argv));

    BOOST_CHECK_EQUAL(echo_receiver().received.size(),   1u);
    BOOST_CHECK_EQUAL(echo_receiver_2().received.size(), 1u);
}

BOOST_FIXTURE_TEST_CASE( unknown_command_case, command_processor_fixture )
{
    const char* argv1[] = { "unknown" };
    BOOST_CHECK(!run(argv1));

    const char* argv2[] = { "ech" };
    BOOST_CHECK(!run(argv2));

    const char* argv3[] = { "echoo" };
    BOOST_CHECK(!run(argv3));

    BOOST_CHECK(echo_receiver().received.empty());
}

BOOST_FIXTURE_TEST_CASE( init_failure_case, command_processor_fixture )
{
    const char* argv[] = { "strict", "a", "b" };
    BOOST_CHECK(!run(argv));

    BOOST_CHECK(strict_receiver().received.empty());
    BOOST_CHECK_EQUAL(out.str(), "strict <arg>\n\r");

    const char* argv_ok[] = { "strict", "a" };
    BOOST_CHECK(run(argv_ok));
    BOOST_REQUIRE_EQUAL(strict_receiver().received.size(), 1u);
    BOOST_CHECK_EQUAL(strict_receiver().received[0].m_arg, "a");
}

BOOST_FIXTURE_TEST_CASE( nested_processor_case, command_processor_fixture )
{
    const char* argv[] = { "nested", "deep", "1", "2" };
    BOOST_CHECK(run(argv));

    BOOST_REQUIRE_EQUAL(deep_receiver().received.size(), 1u);
    BOOST_CHECK_EQUAL(deep_receiver().received[0].m_count, 2u);

    const char* argv_empty[] = { "nested" };
    BOOST_CHECK(!run(argv_empty));
    BOOST_CHECK_EQUAL(deep_receiver().received.size(), 1u);
}

BOOST_AUTO_TEST_CASE( empty_init_case )
{
    processor_t p;
    BOOST_CHECK(!p.init(0, nullptr));
}

BOOST_AUTO_TEST_CASE( help_case )
{
    std::stringstream ss;
    processor_t::help(ss);

    BOOST_CHECK_EQUAL(ss.str(),
        "root\n\r"
        "    echo\n\r"
        "    strict <arg>\n\r"
        "    nested\n\r"
        "        deep\n\r");
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_COMMAND_PROCESSOR_HPP
