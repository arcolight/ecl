#ifndef ECL_SINGLETON_HPP
#define ECL_SINGLETON_HPP

// simple implementation of Meyers singleton

namespace ecl
{

template<typename T>
class singleton : public T
{
public:
    static T& instance();

    // For really lazy people :)
    static T& inst()
    {
        return instance();
    }

    static T* pointer()
    {
        return &(instance());
    }

private:
    singleton() : T() 
    {}
    
    singleton(const singleton<T>& other)                               = delete;
    singleton<T>& operator= (const singleton<T>& other)                = delete;
    singleton(const singleton<T>&& other)                              = delete;
    singleton<T>& operator= (const singleton<T>&& other)               = delete;
};

template<typename T>
T& singleton<T>::instance()
{
    static singleton<T> object;

    return object;
}

template<typename T>
using Singleton = singleton<T>;

} // namespace ecl

#endif // ECL_SINGLETON_HPP
