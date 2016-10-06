#include <iostream>
#include <cstdlib>
#include <string>
#include <ostream>

#include <ecl/map.hpp>

enum class E
{
      e1
    , e2
    , e3
    , e4
    , e5
    , e6
    , e7
    , e8
    , e9
    , e10
};

std::ostream& operator<<(std::ostream& s, E e)
{
    switch(e)
    {
        case E::e1 : s << "e1" ; break;
        case E::e2 : s << "e2" ; break;
        case E::e3 : s << "e3" ; break;
        case E::e4 : s << "e4" ; break;
        case E::e5 : s << "e5" ; break;
        case E::e6 : s << "e6" ; break;
        case E::e7 : s << "e7" ; break;
        case E::e8 : s << "e8" ; break;
        case E::e9 : s << "e9" ; break;
        case E::e10: s << "e10"; break;
        default    :             break;
    }

    return s;
}

using key_type   = E;
using value_type = std::uint32_t;

using map_t      = ecl::map<key_type, value_type, 16>;

 auto m0 = ecl::create_map<key_type, value_type>
 (
       std::make_pair(E::e1, 1u)
     , std::make_pair(E::e2, 2u)
     , std::make_pair(E::e3, 4u)
     , std::make_pair(E::e3, 3u) // overwrite E::e3 element
 );

template<typename T>
void dump_map(std::string prefix, T& m)
{
    std::cout << prefix << " map elements count: " << m.size() << std::endl;

    std::cout << prefix << " range-for interation over map: ";
    for(auto& p : m)
    {
        std::cout << "(" << p.first << ":" << p.second << ") ";
    }
    std::cout << std::endl;

    std::cout << prefix << "[E::e1]:  " << m[E::e1]  << std::endl;
    std::cout << prefix << "[E::e2]:  " << m[E::e2]  << std::endl;
    std::cout << prefix << "[E::e3]:  " << m[E::e3]  << std::endl;
    std::cout << prefix << "[E::e4]:  " << m[E::e4]  << std::endl;
    std::cout << prefix << "[E::e5]:  " << m[E::e5]  << std::endl;
    std::cout << prefix << "[E::e6]:  " << m[E::e6]  << std::endl;
    std::cout << prefix << "[E::e7]:  " << m[E::e7]  << std::endl;
    std::cout << prefix << "[E::e8]:  " << m[E::e8]  << std::endl;
    std::cout << prefix << "[E::e9]:  " << m[E::e9]  << std::endl;
    std::cout << prefix << "[E::e10]: " << m[E::e10] << std::endl;
}

int main(int, char**, char**)
{
    map_t m1
    {
          { E::e1  , 2  }
        , { E::e2  , 4  }
        , { E::e3  , 6  }
        , { E::e4  , 8  }
        , { E::e5  , 10 }
        , { E::e6  , 12 }
        , { E::e7  , 14 }
        , { E::e8  , 16 }
        , { E::e9  , 18 }
        , { E::e10 , 20 }
    };

    map_t m2;

    m2[E::e1] = 1;

    dump_map("[m1]", m1);
    auto i = m1.begin();
    std::advance(i, 0);
    m1.erase(i, m1.end());
    dump_map("[m1]", m1);

    return 0;
}
