#ifndef ECL_WEB_SERVER_HPP
#define ECL_WEB_SERVER_HPP

#include <ecl/stream.hpp>
#include <ecl/web/request_parser.hpp>

namespace ecl
{

namespace web
{

template<typename RESOURCES>
class server
{
public:
    using resources_t = RESOURCES;

    template<typename STREAM>
    void process_request(char* req_raw, std::size_t size, STREAM& st)
    {
        request* req = m_parser.parse(req_raw, size);
        do
        {
            req = m_resources.template call<STREAM>(st, req);
        }
        while(req != nullptr);
    }

    resources_t    m_resources {};
    request_parser m_parser    {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_SERVER_HPP
