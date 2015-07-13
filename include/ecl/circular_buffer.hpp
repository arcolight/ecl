/**
 * @file
 *
 * @brief Circular buffer class.
 *
 * @ingroup ecl
 */
#ifndef ECL_CIRCULAR_BUFFER_HPP
#define ECL_CIRCULAR_BUFFER_HPP

#include <cstddef>
#include <type_traits>
#include <iterator>

namespace ecl
{

/**
 * @brief Circular buffer class.
 *
 * @tparam T Type of elements.
 * @tparam CAPACITY Buffer capacity in elements.
 * @tparam ERASE_ELEMENTS = false Determines delete or not elements on
 * @ref clear
 */
template<typename T, std::size_t CAPACITY, bool ERASE_ELEMENTS = false>
class circular_buffer
{
public:
    circular_buffer() :
    m_offset(0),
    m_size(0)
    {
        static_assert(std::is_nothrow_copy_constructible<T>::value,
                      "T must be nothrow copy-constructible!");
        static_assert(std::is_pod<T>::value, "T must be POD!");
        static_assert(CAPACITY > 0, "Size must be greater than zero!");
    }

    /**
     * @brief Clear buffer.
     *
     */
    void clear()
    {
        if(ERASE_ELEMENTS)
        {
            for(auto& c: m_array)
            {
                c = T();
            }
        }

        m_offset = 0;
        m_size = 0;
    }

    /**
     * @brief Circular buffer capacity.
     * @return Circular buffer capacity.
     */
    constexpr static std::size_t capacity()
    {
        return CAPACITY;
    }

    /**
     * @brief Circular buffer current size;
     * @return Current elements count in buffer.
     */
    std::size_t size()                                                     const
    {
        return m_size;
    }

    /**
     * @brief Push elemnt to buffer.
     *
     * @param v Element to push.
     */
    void push(const T& v)
    {
        m_array[wrap(m_offset + m_size)] = v;

        if(m_size < CAPACITY)
        {
            ++m_size;
        }
        else
        {
            m_offset = wrap(m_offset + 1);
        }
    }

    /**
     * @brief Extract element from buffer
     *
     * @return Top element.
     */
    T pop()
    {
        T t = m_array[m_offset];
        if(ERASE_ELEMENTS)
        {
            m_array[m_offset] = T();
        }

        if(m_size != 0)
        {
            --m_size;
            m_offset = wrap(m_offset + 1);
            return t;
        }

        return T();
    }

    T& operator[](std::size_t index)
    {
        return m_array[wrap(index + m_offset)];
    }

    const T& operator[](std::size_t index)                                 const
    {
        return m_array[wrap(index + m_offset)];
    }

    bool is_empty()                                                        const
    {
        return (m_size == 0);
    }

    T& front()
    {
        return operator [](0);
    }

    const T& front()                                                       const
    {
        return operator [](0);
    }

    T& back()
    {
        if(is_empty())
        {
            return front();
        }

        return operator [](m_size - 1);
    }

    const T& back()                                                        const
    {
        if(is_empty())
        {
            return front();
        }

        return operator [](m_size - 1);
    }

    void drop_front(std::size_t count)
    {
        if (m_size <= count)
        {
            clear();
            return;
        }

        if(ERASE_ELEMENTS)
        {
            for(std::size_t i = 0; i < count; ++i)
            {
                m_array[wrap(m_offset + i)] = T();
            }
        }

        m_offset = wrap(m_offset + count);
        m_size -= count;
    }

    void drop_back(std::size_t count)
    {
        if (m_size <= count)
        {
            clear();
            return;
        }

        if(ERASE_ELEMENTS)
        {
            for(std::size_t i = 0; i < count; ++i)
            {
                m_array[wrap(m_offset + m_size - i - 1)] = T();
            }
        }

        m_size -= count;
    }

    class iterator
    {
        public:
            typedef iterator self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;
            iterator(std::size_t ind, circular_buffer<T,
                                                      CAPACITY,
                                                      ERASE_ELEMENTS>& data) :
                index_(ind),
                data_(data)
            {}

            self_type& operator++()
            {
                if(CAPACITY == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return *this;
            }

            self_type operator++(int junk)
            {
                (void)(junk);

                if(CAPACITY == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return *this;
            }

            self_type& operator--()
            {
                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }

                return *this;
            }

            self_type operator--(int junk)
            {
                (void)(junk);
                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }
                return *this;
            }

            reference operator*()
            {
                return data_[index_];
            }

            bool operator==(const self_type& rhs)                          const
            {
                return index_ == rhs.index_;
            }

            bool operator!=(const self_type& rhs)                          const
            {
                return !operator==(rhs);
            }

        private:
            std::size_t                                   index_;
            circular_buffer<T, CAPACITY, ERASE_ELEMENTS>& data_;
    };

    class const_iterator
    {
        public:
            typedef const_iterator self_type;
            typedef const T value_type;
            typedef const T& reference;
            typedef const T* pointer;
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;
            const_iterator(std::size_t ind,
                           const       circular_buffer<T,
                                                       CAPACITY,
                                                       ERASE_ELEMENTS>& data) :
                index_(ind),
                data_(data)
            {}

            self_type& operator++()
            {
                if(CAPACITY == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return *this;
            }

            self_type operator++(int junk)
            {
                (void)(junk);

                if(CAPACITY == index_)
                {
                    index_ = 0;
                }
                else
                {
                    ++index_;
                }

                return *this;
            }

            self_type& operator--()
            {
                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }

                return *this;
            }

            self_type operator--(int junk)
            {
                (void)(junk);
                if(0 == index_)
                {
                    index_ = CAPACITY;
                }
                else
                {
                    --index_;
                }
                return *this;
            }

            reference operator*()                                          const
            {
                return data_[index_];
            }

            bool operator==(const self_type& rhs)                          const
            {
                return index_ == rhs.index_;
            }

            bool operator!=(const self_type& rhs)                          const
            {
                return !operator==(rhs);
            }

        private:
            std::size_t                                         index_;
            const circular_buffer<T, CAPACITY, ERASE_ELEMENTS>& data_;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin()
    {
        return iterator(0, *this);
    }

    iterator end()
    {
        return iterator(m_size, *this);
    }

    const_iterator begin()                                                 const
    {
        return const_iterator(0, *this);
    }

    const_iterator end()                                                   const
    {
        return const_iterator(m_size, *this);
    }

    reverse_iterator rbegin()
    {
        return ++reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return ++reverse_iterator(begin());
    }

    const_reverse_iterator rbegin()                                        const
    {
        return ++const_reverse_iterator(end());
    }

    const_reverse_iterator rend()                                          const
    {
        return ++const_reverse_iterator(begin());
    }

private:
    static std::size_t wrap(std::size_t i)
    {
        return i % CAPACITY;
    }

    std::size_t m_offset;
    std::size_t m_size;
    T           m_array[CAPACITY];
};

} // namespace ecl

#endif // ECL_CIRCULAR_BUFFER_HPP
