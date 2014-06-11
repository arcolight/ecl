#ifndef ECL_CB_HPP
#define ECL_CB_HPP

#include <cstddef>
#include <type_traits>

namespace ecl
{

template<typename T, size_t SIZE>
class circular_buffer
{
public:
    explicit circular_buffer(const T& def) :
        m_offset(0),
        m_count(0),
        m_default(def)
    {
        static_assert(std::is_nothrow_copy_constructible<T>::value,
                      "T must be nothrow copy-constructible!");
        static_assert(std::is_pod<T>::value, "T must be POD!");
        static_assert(SIZE > 0, "Size must be greater than zero!");

        clear();
    }

    void clear()                                                
    {
        for(T& s: m_array) {
            s = m_default;
        }

        m_offset = 0;
        m_count = 0;
    }

    size_t size()                                                          const
    {
        return SIZE;
    }

    size_t count()                                                         const
    {
        return m_count;
    }

    size_t push(const T& v)                                     
    {
        m_array[m_offset] = v;
        m_offset = wrap(m_offset + 1);

        if(m_count < SIZE) {
            ++m_count;
        }

        return m_offset;
    }

    T pop()                                                     
    {
        m_offset = wrap(m_offset - 1);
        T t = m_array[m_offset];
        m_array[m_offset] = m_default;

        if(m_count != 0) {
            --m_count;
        }

        return t;
    }

    T& operator[](size_t index)
    {
#ifdef CB_RAW_ACCESS
        return m_array[wrap(index)];
#else
        return m_array[wrap(index + m_offset)];
#endif
    }

    const T& operator[](size_t index)                                      const
    {
#ifdef CB_RAW_ACCESS
        return m_array[wrap(index)];
#else
        return m_array[wrap(index + m_offset)];
#endif
    }

    class iterator 
    {
        public:
            typedef iterator self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            // typedef std::forward_iterator_tag iterator_category;
            typedef int difference_type;
            iterator(size_t ind, pointer data, size_t offset) : 
                index_(ind), 
                offset_(offset),
                data_(data)
            {}

            self_type operator++() 
            {
                self_type i = *this;
                ++index_;
                return i;
            }

            self_type operator++(int junk)
            {
                ++index_;
                return *this; 
            }

            reference operator*()
            {
                return data_[wrap(index_ + offset_)];
            }

            pointer operator->() 
            {   
                return data_; 
            }

            bool operator==(const self_type& rhs)                          const
            {
                return index_ == rhs.index_;
            }

            bool operator!=(const self_type& rhs)                          const
            {
                return index_ != rhs.index_;
            }

        private: 
            size_t  index_;
            size_t  offset_;
            pointer data_;
    };

    class const_iterator 
    {
        public:
            typedef iterator self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            // typedef std::forward_iterator_tag iterator_category;
            typedef int difference_type;
            const_iterator(size_t ind, pointer data, size_t offset) : 
                index_(ind), 
                offset_(offset),
                data_(data)
            {}

            self_type operator++() 
            {
                self_type i = *this;
                ++index_;
                return i;
            }

            self_type operator++(int junk)
            {
                ++index_;
                return *this; 
            }

            reference operator*()                                          const
            {
                return data_[wrap(index_ + offset_)];
            }

            pointer operator->() 
            {   
                return data_; 
            }

            bool operator==(const self_type& rhs)                          const
            {
                return index_ == rhs.index_;
            }

            bool operator!=(const self_type& rhs)                          const
            {
                return index_ != rhs.index_;
            }

        private: 
            size_t  index_;
            size_t  offset_;
            pointer data_;
    };

    iterator begin()
    {
        return iterator(0, m_array, m_offset);
    }

    iterator end()
    {
        return iterator(SIZE, m_array, m_offset);
    }

    const_iterator begin()                                                 const
    {
        return const_iterator(0, m_array, m_offset);
    }

    const_iterator end()                                                   const
    {
        return const_iterator(SIZE, m_array, m_offset);
    }

private:
    static size_t wrap(size_t i)
    {
        return i % SIZE;
    }

    size_t to_real_index(size_t i)
    {
        return wrap(i + m_offset);
    }

    size_t  m_offset;
    size_t  m_count;
    T       m_array[SIZE];
    const T m_default;
};

} // namespace ecl

#endif // ECL_CB_HPP
