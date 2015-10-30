#ifndef ECL_WEB_SERVER_HPP
#define ECL_WEB_SERVER_HPP

#include <ecl/stream.hpp>
#include <ecl/web/request_parser.hpp>

namespace ecl
{

namespace web
{

template<typename RESOURCES, std::size_t MAX_REQUEST_SIZE>
class server
{
public:
    using resources_t = RESOURCES;

    template<typename STREAM>
    void process_request(const char* req_raw, std::size_t, STREAM& st)
    {
        do
        {
            req_raw = m_resources.template call<STREAM>(st,
                                                        m_parser.parse(req_raw));
        }
        while(req_raw != nullptr);
    }

    resources_t                      m_resources {};
    request_parser<MAX_REQUEST_SIZE> m_parser    {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_SERVER_HPP
