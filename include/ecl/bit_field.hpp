/**
 * @file
 *
 * @brief Bit field class.
 * @details Bit field class. Endianness independent. Compiler independent.
 *
 * @ingroup ecl
 */
#ifndef ECL_BIT_FIELD_HPP
#define ECL_BIT_FIELD_HPP

#include <cstdint>
#include <cstdlib>

#define ECL_FIELD(TYPE, NAME, SIZE) \
    ecl::field<TYPE, decltype(TYPE::NAME), &TYPE::NAME, SIZE>

namespace ecl
{

/**
 * @brief Field class.
 * @details Field class. Describes one field in bit-field. Should be used as
 * template argument for @ref bit_field class.
 *
 * @tparam S Base struct type.
 * @tparam T Type of the field.
 * @tparam P Name of the field in the base struct S.
 * @tparam SIZE Size in bits(!).
 */
template<typename S, typename T, T S::*P, std::size_t SIZE>
class field : public virtual S
{
public:
    field()                                                             noexcept
    {
        static_assert((sizeof(T) * 8 >= SIZE), "field size > sizeof(var)");
    }

    constexpr static std::size_t size()                                 noexcept
    {
        return SIZE;
    }

protected:
    using field_t = T;

    constexpr field_t get()                                       const noexcept
    {
        return this->*P;
    }

    void set(const field_t& val)                                        noexcept
    {
        this->*P = val;
    }
};

namespace
{
    template<typename F, typename... Fs>
    constexpr std::size_t calc_size()
    {
        return F::size() + calc_size<Fs...>();
    }

    template<>
    constexpr std::size_t calc_size<void>()
    {
        return 0;
    }

    template<typename... Fs>
    constexpr std::size_t size_in_bytes()
    {
        return (calc_size<Fs..., void>() + 7) / 8;
    }
}

/**
 * @brief bit_field class.
 * @details bit_field class. Bit field class can pack struct into byte array,
 * according to fields definition, allowing to work with fields like with the
 * struct fields.
 *
 * @tparam BASE Base struct type.
 * @tparam FIELDS List of the fields.
 */
template <typename BASE, typename... FIELDS>
class bit_field final : public FIELDS...
{
public:
    constexpr static std::size_t size { size_in_bytes<FIELDS...>() };

    struct packed_data
    {
        uint8_t data[size];
    };

    using packed_data_t = struct packed_data;

    bit_field () : FIELDS()...
    {
    }

    bit_field(const bit_field<BASE, FIELDS...>& other)
    {
        set_data(other);
    }

    operator uint8_t*()                                           const noexcept
    {
        return m_array;
    }

    operator const uint8_t*()                                     const noexcept
    {
        return m_array;
    }

    bit_field<BASE, FIELDS...>&
    operator= (const bit_field<BASE, FIELDS...>& other)
    {
        set_data(other);
        return *this;
    }

    /**
     * @brief Set fields from byte array.
     * @details Set fields from byte array. Unpack array according to fields
     * definitions and set appropriate fields.
     *
     * @param data Pointer to byte array.
     * @attention No size check fir @ref data !
     */
    void set_data(const uint8_t* const data)                            noexcept
    {
        for(std::size_t i = 0; i < size; ++i)
        {
            m_array[i] = data[i];
        }

        unpack();
    }

    // 'm_array - 1' - Hack :( Needed more graceful solution.
    // Disable GCC warning, we know what we do.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    const uint8_t* pack()                                               noexcept
    {
        clear_array();
        pack_<0, FIELDS...>(&m_array[0] - 1);
        return m_array;
    }

    /**
     * @brief Unpacks internal byte array to fields.
     * @details Unpacks internal byte array to fields.
     * @return Pointer to base struct.
     */
    BASE* unpack()                                                      noexcept
    {
        unpack_<0, FIELDS...>(&m_array[0] - 1);
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

private:
    template<uint8_t OFFSET>
    void pack_(uint8_t* /* array */)                              const noexcept
    {
        static_assert((OFFSET <= size * 8), "offset out of bound");
    }

    template<uint8_t OFFSET, typename F, typename... TAIL>
    void pack_(uint8_t* array)                                          noexcept
    {
        typename F::field_t val = this->F::get();
        uint8_t bit = 0x00;

        for(std::size_t i = 0; i < F::size(); ++i)
        {
            std::size_t shift = (OFFSET + i) % 8;
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
    void unpack_(uint8_t* /* array */)                            const noexcept
    {
        static_assert((OFFSET <= size * 8), "offset out of bound");
    }

    template<uint8_t OFFSET, typename F, typename... TAIL>
    void unpack_(uint8_t* array)                                        noexcept
    {
        typename F::field_t val = 0;
        decltype(val) bit = 0;

        for(std::size_t i = 0; i < F::size(); ++i)
        {
            std::size_t shift = (OFFSET + i) % 8;
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

    uint8_t m_array[size] {};
};

} // namespace ecl

#endif //ECL_BIT_FIELD_HPP
