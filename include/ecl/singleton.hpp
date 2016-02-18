#ifndef ECL_SINGLETON_HPP
#define ECL_SINGLETON_HPP

// simple implementation of Meyers singleton

namespace ecl
{

template<typename T>
class singleton : public T
{
public:
    static T& instance()                                               noexcept;

    // For really lazy people :)
    static T& inst()                                                    noexcept
    {
        return instance();
    }

    static T* pointer()                                                 noexcept
    {
        return &(instance());
    }

private:
    singleton()                                                         noexcept
        : T()
    {}

    singleton(const singleton<T>& other)                               = delete;
    singleton<T>& operator= (const singleton<T>& other)                = delete;
    singleton(const singleton<T>&& other)                              = delete;
    singleton<T>& operator= (const singleton<T>&& other)               = delete;
};

template<typename T>
T& singleton<T>::instance()                                             noexcept
{
    static singleton<T> object;

    return object;
}

template<typename T>
using Singleton = singleton<T>;

} // namespace ecl

#endif // ECL_SINGLETON_HPP
