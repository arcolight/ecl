#pragma once

#include <type_traits>
#include <array>
#include <tuple>
#include <cstdint>

#include "str_const.hpp"

namespace ecl
{

namespace json
{

bool write_to_buf(const char* const NAME,
                  bool&             val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{

    return true;
}

bool write_to_buf(const char* const NAME, 
                  int8_t&           val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME, 
                  uint8_t&          val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME, 
                  int16_t&          val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME, 
                  uint16_t&         val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME, 
                  int32_t&          val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME,
                  uint32_t&         val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME,
                  int64_t&          val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME,
                  uint64_t&         val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

bool write_to_buf(const char* const NAME,
                  const char*       val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

template<typename T>
bool write_to_buf(const char* const NAME,
                  T&                val,
                  uint8_t*          buf,
                  size_t            sz,
                  uintptr_t&        offset)
{
    return true;
}

template<const char* const NAME, typename VAL_TYPE>
class node
{
public:
	bool serialize(uint8_t* buf, size_t sz, uintptr_t& offset)    			                   const
	{
        return serialize_internal(buf, sz, offset);
	}

private:
    bool serialize_internal(uint8_t* buf, size_t sz, uintptr_t& offset)                             const
    {
        return write_to_buf(NAME, m_val, buf, sz, offset);
    }

	VAL_TYPE m_val;
};

template<typename... NODES>
class object
{
public:
	bool serialize(uint8_t* buf, size_t sz)                                const
	{
        uintptr_t offset = 0;
        return serialize_internal<0, NODES...>(buf, sz, offset);
	}

private:
	template<size_t COUNT, typename NODE, typename... TAIL>
	bool serialize_internal(uint8_t* buf, size_t sz, uintptr_t& offset)	   const
	{
        std::get<COUNT>(m_nodes).serialize(buf + offset, sz - offset, offset);

		return serialize_internal<COUNT + 1, TAIL...>(buf + offset, sz - offset, offset);
	}

	template<size_t COUNT>
	bool serialize_internal(uint8_t* buf, size_t sz, uintptr_t& offset)	   const
	{
		return true;
	}

    std::tuple<NODES...> m_nodes;
};

} // namespace json
	
} // namespace ecl
