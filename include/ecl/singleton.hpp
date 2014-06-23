#ifndef ECL_SINGLETON_HPP
#define ECL_SINGLETON_HPP

// simple implementation of Meyers singleton

namespace ecl
{

template<typename T>
class singleton
{
public:
    static T& instance()
    {
        static T t;
        return t;
    }
};
    
} // namespace ecl

#endif // ECL_SINGLETON_HPP
