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
    void process_request(STREAM& st, const char* req_raw, std::size_t size)
    {
        m_resources.template call<STREAM>(st, m_parser.parse(req_raw, size));
    }

    resources_t                      m_resources {};
    request_parser<MAX_REQUEST_SIZE> m_parser    {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_SERVER_HPP
