#ifndef ECL_JSON_NODE_HPP
#define ECL_JSON_NODE_HPP

#include <ecl/json/helpers.hpp>
#include <ecl/json/object.hpp>

namespace ecl
{

namespace json
{

template<typename NAME, typename T>
class node
{
public:
    constexpr node() {}

    typedef T    value_t;
    typedef NAME name_t;

protected:
    template<typename STREAM>
    void serialize(STREAM& st,
                   bool beautify,
                   size_t indent)                                          const
    {
        stringify(st, NAME::name(), beautify, indent);
        st << ':';
        stringify(st, m_val, beautify, indent);
    }

    bool deserialize(const char*& s)
    {
        spaces_rollup(s);
        if(*s != '"')
        {
            return false;
        }
        s++;

        if(0 != strncmp(s, NAME::name(), NAME::size()))
        {
            return false;
        }
        s += NAME::size();

        spaces_rollup(s);
        if(*s != '"')
        {
            return false;
        }
        s++;

        spaces_rollup(s);
        if(*s != ':')
        {
            return false;
        }
        s++;

        if(!val_deserializer<value_t>::parse(s, m_val))
        {
            return false;
        }

        return true;
    }

    // 1 -     ':'
    // 2 - 2 * '"'
    constexpr static size_t size()
    {
        return 1 + 2 + NAME::size() + val_size<value_t>::size();
    }

    value_t m_val { val_initializer<value_t>::value() };

private:
    template<typename STREAM, typename V_T>
    void stringify(STREAM& st,
                   const V_T& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        val.serialize(st, beautify, indent);
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const bool& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const int8_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const uint8_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const int16_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const uint16_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const int32_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const uint32_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const int64_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const uint64_t& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const float& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const double& val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << val;
    }

    template<typename STREAM, size_t N>
    void stringify(STREAM& st,
                   const ecl::json::string<N>& str,
                   bool beautify,
                   size_t indent)                                          const
    {
        stringify(st, str.data(), beautify, indent);
    }

    template<typename STREAM>
    void stringify(STREAM& st,
                   const char* val,
                   bool beautify,
                   size_t indent)                                          const
    {
        (void)beautify;
        (void)indent;
        st << "\"";

        for(size_t i = 0; i < strlen(val); ++i)
        {
            if(val[i] == '"'  ||
               val[i] == '\\' ||
               val[i] == '/')
            {
                st << "\\";
                st << val[i];
            }
            else if(val[i] == '\b')
            {
                st << "\\b";
            }
            else if(val[i] == '\f')
            {
                st << "\\f";
            }
            else if(val[i] == '\n')
            {
                st << "\\n";
            }
            else if(val[i] == '\r')
            {
                st << "\\r";
            }
            else if(val[i] == '\t')
            {
                st << "\\t";
            }
            else
            {
                st << val[i];
            }
        }

        st << "\"";
    }

#ifdef ECL_WITH_STD_STRING
    template<typename STREAM>
    void stringify(STREAM& st,
                   const std::string& str,
                   bool beautify,
                   size_t indent)                                          const
    {
        stringify(st, str.data(), beautify, indent);
    }
#endif

};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_NODE_HPP
