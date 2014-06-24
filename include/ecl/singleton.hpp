#ifndef ECL_SINGLETON_HPP
#define ECL_SINGLETON_HPP

// simple implementation of Meyers singleton

namespace ecl
{

template<typename T>
class singleton
{
public:
    virtual ~singleton() {}

    static T& instance()
    {
        static T object;

        return object;
    }

    // For really lazy people :)
    static T& inst()
    {
        return instance();
    }

    static T* pointer()
    {
        return &(instance());
    }

protected:
    singleton() {}

private:
    singleton(const singleton<T>& other)                               = delete;
    singleton<T>& operator= (const singleton<T>& other)                = delete;
    singleton(const singleton<T>&& other)                              = delete;
    singleton<T>& operator= (const singleton<T>&& other)               = delete;
};

template<typename T>
using Singleton = singleton<T>;

} // namespace ecl

#endif // ECL_SINGLETON_HPP
