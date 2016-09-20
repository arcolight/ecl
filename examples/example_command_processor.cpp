#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

#include <ecl/name_type.hpp>
#include <ecl/command.hpp>
#include <ecl/command_processor.hpp>
#include <ecl/stream.hpp>

#define MAX_TOKENS 64

ECL_DECL_NAME_TYPE(proc)
ECL_DECL_NAME_TYPE(sub)
ECL_DECL_NAME_TYPE(cmd1)
ECL_DECL_NAME_TYPE(cmd2)
ECL_DECL_NAME_TYPE(cmd3)
ECL_DECL_NAME_TYPE(cmd4)
ECL_DECL_NAME_TYPE(cmd5)
ECL_DECL_NAME_TYPE(quit)

namespace
{

struct command_quit : public ecl::command<quit, command_quit>
{
    bool init(const std::size_t           argc,
              const char* const* const    argv)
    {
        if(argc > 0)
        {
            m_exit_code = static_cast<int>(std::strtol(argv[0], nullptr, 10));
        }

        return true;
    }

    template<typename ST>
    static void show_help(ST& st, size_t indent, size_t indent_increment = DEFAULT_INDENT_INCREMENT)
    {
        (void)indent_increment;
        print_indent(st, indent);
        st << name() << " <exit code>" << "\n\r";
    }

    int m_exit_code { 0 };
};

struct command_1    : public ecl::command<cmd1, command_1>
{
    bool init(const std::size_t           argc,
              const char* const* const    argv)
    {
        for(size_t i = 0; i < argc; ++i)
        {
            m_args.push_back(argv[i]);
        }

        return true;
    }

    std::vector<std::string> m_args {};
};

struct command_2    : public ecl::command<cmd2, command_2>
{
    bool init(const std::size_t           argc,
              const char* const* const    argv)
    {
        for(size_t i = 0; i < argc; ++i)
        {
            m_args.push_back(argv[i]);
        }

        return true;
    }

    std::vector<std::string> m_args {};
};

struct command_3    : public ecl::command<cmd3, command_3>
{
    bool init(const std::size_t           argc,
              const char* const* const    argv)
    {
        for(size_t i = 0; i < argc; ++i)
        {
            m_args.push_back(argv[i]);
        }

        return true;
    }

    std::vector<std::string> m_args {};
};

struct command_4    : public ecl::command<cmd4, command_4>
{
    bool init(const std::size_t           argc,
              const char* const* const    argv)
    {
        for(size_t i = 0; i < argc; ++i)
        {
            m_args.push_back(argv[i]);
        }

        return true;
    }

    std::vector<std::string> m_args {};
};

struct command_5    : public ecl::command<cmd5, command_5>
{
    bool init(const std::size_t           argc,
              const char* const* const    argv)
    {
        for(size_t i = 0; i < argc; ++i)
        {
            m_args.push_back(argv[i]);
        }

        return true;
    }

    std::vector<std::string> m_args {};
};

}

typedef ecl::command_processor< proc,
    command_quit,
    command_1,
    command_2,
    ecl::command_processor< sub,
        command_3,
        command_4,
        command_5
    >
> processor_t;

namespace
{

class receiver_quit : public ecl::receiver<command_quit>
{
public:
    virtual void receive(command_quit& q)
    {
        std::cout << "Quit command received. code: " << q.m_exit_code << std::endl;
        exit(q.m_exit_code);
    }
};

class receiver_1 : public ecl::receiver<command_1>
{
public:
    virtual void receive(command_1& c)
    {
        std::cout << "Receiver 1 <- Command 1 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }
};

class receiver_2 : public ecl::receiver<command_2>
{
public:
    virtual void receive(command_2& c)
    {
        std::cout << "Receiver 2 <- Command 2 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }
};

class receiver_3 : public ecl::receiver<command_1>,
                   public ecl::receiver<command_2>,
                   public ecl::receiver<command_3>
{
public:
    virtual void receive(command_1& c)
    {
        std::cout << "Receiver 3 <- Command 1 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }

    virtual void receive(command_2& c)
    {
        std::cout << "Receiver 3 <- Command 2 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }

    virtual void receive(command_3& c)
    {
        std::cout << "Receiver 3 <- Command 3 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }
};

class receiver_4 : public ecl::receiver<command_3>,
                   public ecl::receiver<command_4>,
                   public ecl::receiver<command_5>
{
public:
    virtual void receive(command_3& c)
    {
        std::cout << "Receiver 4 <- Command 3 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }

    virtual void receive(command_4& c)
    {
        std::cout << "Receiver 4 <- Command 4 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }

    virtual void receive(command_5& c)
    {
        std::cout << "Receiver 4 <- Command 5 with " << c.m_args.size() << " args:" << std::endl;
        for(auto& s : c.m_args)
        {
            std::cout << s << std::endl;
        }
    }
};

}

using help_stream_t = ecl::stream<1024>;

int main(int, char**, char**)
{
    receiver_1    r1;
    receiver_2    r2;
    receiver_3    r3;
    receiver_4    r4;
    receiver_quit rq;

    processor_t proc;

    std::cout << "Starting command processor..." << std::endl;

    while(true)
    {
        std::cout << "proc > ";
        std::string input;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> tokens(begin, end);

        size_t token_count = std::min(static_cast<size_t>(MAX_TOKENS), tokens.size());

        tokens.resize(token_count);
        tokens.shrink_to_fit();

        size_t ac = tokens.size();
        const char* av[MAX_TOKENS];

        for(size_t i = 0; i < token_count; ++i)
        {
            av[i] = tokens[i].data();
        }

        help_stream_t st;

        if(proc.init(ac, av))
        {
            if(!proc.dispatch(st))
            {
//                proc.help(st);
                std::cout << st << std::endl;
            }
        }
        else
        {
            proc.help(st);
            std::cout << st << std::endl;
        }
    }
}
