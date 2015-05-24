#ifndef ECL_BIT_FIELD_HPP
#define ECL_BIT_FIELD_HPP

#include <cstdint>
#include <cstdlib>

#define ECL_FIELD(TYPE, NAME, SIZE) \
    ecl::field<TYPE, decltype(TYPE::NAME), &TYPE::NAME, SIZE>

namespace ecl
{

template<typename S, typename T, T S::*P, size_t SIZE>
class field : public virtual S
{
public:
    constexpr field() 
    {
        static_assert((sizeof(T) * 8 >= SIZE), "field size > sizeof(var)");
    }

    constexpr static size_t size()
    {
        return SIZE;
    }

protected:
    typedef T field_t;

    constexpr field_t get()                                                const
    {
        return this->*P;
    }

    void set(const field_t& val)
    {
        this->*P = val;
    }
};

template <size_t SIZE, typename BASE, typename... FIELDS>
class bit_field final : public FIELDS...
{
public:
    typedef struct packed_data 
    {
        uint8_t data[SIZE];
    } packed_data_t;

    bit_field () : FIELDS()...
    {
        check<0, FIELDS...>();
    }

    bit_field(const bit_field<SIZE, BASE, FIELDS...>& other)
    {
        set_data(other);
    }

    operator uint8_t*()                                                    const
    {
        return m_array;
    }

    operator const uint8_t*()                                              const
    {
        return m_array;
    }

    bit_field<SIZE, BASE, FIELDS...>&
    operator= (const bit_field<SIZE, BASE, FIELDS...>& other)
    {
        set_data(other);
        return *this;
    }

    void set_data(const uint8_t* const data)
    {
        for(size_t i = 0; i < SIZE; ++i) 
        {
            m_array[i] = data[i];
        }

        unpack();
    }

    // 'm_array - 1' - Hack :( Needed more graceful solution.
    // Disable GCC warning, we know what we do.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    const uint8_t* pack()
    {
        clear_array();
        pack_<0, FIELDS...>(m_array - 1);
        return m_array;
    }

    BASE* unpack()
    {
        unpack_<0, FIELDS...>(m_array - 1);
        return this;
    }
#pragma GCC diagnostic pop

    void clear_array()
    {
        for(uint8_t& byte: m_array)
        {
            byte = 0x00;
        }
    }

    constexpr static size_t size { SIZE };

private:
    template<size_t SUM>
    constexpr bool check()                                                 const
    {
        static_assert((SIZE * 8 >= SUM), "array to small");
        return true;
    }

    template<size_t SUM, typename F, typename... TAIL>
    constexpr bool check()                                                 const
    {
        return check<SUM + F::size(), TAIL...>();
    }

    template<uint8_t OFFSET>
    void pack_(uint8_t* array)                                             const
    {
        static_assert((OFFSET <= SIZE * 8), "offset out of bound");
        (void)array;
    }

    template<uint8_t OFFSET, typename F, typename... TAIL>
    void pack_(uint8_t* array)                                             const
    {
        typename F::field_t val = this->F::get();
        uint8_t bit = 0x00;

        for(size_t i = 0; i < F::size(); ++i)
        {
            size_t shift = (OFFSET + i) % 8;
            if(0 == shift)
            {
                ++array;
            }

            bit = ( val >> (F::size() - i - 1) ) & 0x01;
            *array |= bit << (7 - shift);
        }

        pack_<(OFFSET + F::size()) % 8, TAIL...>(array);
    }

    template<uint8_t OFFSET>
    void unpack_(uint8_t* array)                                           const
    {
        static_assert((OFFSET <= SIZE * 8), "offset out of bound");
        (void)array;
    }

    template<uint8_t OFFSET, typename F, typename... TAIL>
    void unpack_(uint8_t* array)
    {
        typename F::field_t val = 0;
        decltype(val) bit = 0;

        for(size_t i = 0; i < F::size(); ++i)
        {
            size_t shift = (OFFSET + i) % 8;
            if(0 == shift) 
            {
                ++array;
            }

            bit = ( *array >> (7 - shift) ) & 0x01;
            val |= bit << (F::size() - i - 1);
        }

        this->F::set(val);

        unpack_<(OFFSET + F::size()) % 8, TAIL...>(array);
    }

    uint8_t m_array[SIZE];
};

} // namespace ecl

#endif //ECL_BIT_FIELD_HPP
