#include <ecl/map.hpp>

#include <iostream>
#include <cstdlib>
#include <string>
#include <ostream>

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

using map_t      = ecl::map<key_type, value_type, 8>;

const auto m0 = ecl::create_map<key_type, value_type>
(
      std::make_pair(E::e1, 1u)
    , std::make_pair(E::e2, 2u)
    , std::make_pair(E::e3, 4u)
    , std::make_pair(E::e3, 3u) // overwrite E::e3 element
);

template<typename T>
void fill_map_ilist(std::string prefix, T& m)
{
    std::cout << ">>> Insert to map" << std::endl;
    m.insert(
    {
          { E::e1  , 1  }
        , { E::e2  , 3  }
        , { E::e3  , 5  }
        , { E::e4  , 7  }
        , { E::e5  , 9  }
        , { E::e6  , 11 }
        , { E::e7  , 13 }
        , { E::e8  , 15 }
        , { E::e9  , 17 }
        , { E::e10 , 19 }
    });

    dump_map(prefix, m);
}

template<typename T>
void fill_map_operator(std::string prefix, T& m)
{
    std::cout << ">>> Assign via operator[]" << std::endl;

    m[E::e1]  = 5;
    m[E::e2]  = 10;
    m[E::e3]  = 15;
    m[E::e4]  = 20;
    m[E::e5]  = 25;
    m[E::e6]  = 30;
    m[E::e7]  = 35;
    m[E::e8]  = 40;
    m[E::e9]  = 45;
    m[E::e10] = 50;

    dump_map(prefix, m);
}

template<typename T>
void dump_map(std::string prefix, T& m)
{
    std::cout << ">>> Dump map" << std::endl;
    std::cout << prefix << " map elements size:     " << m.size()     << std::endl;
    std::cout << prefix << " map elements max_size: " << m.max_size() << std::endl;

    std::cout << prefix << " range-for interation over map: ";
    for(auto& p : m)
    {
        std::cout << "(" << p.first << ":" << p.second << ") ";
    }
    std::cout << std::endl;

    std::cout << prefix << ".at(E::e1):  " << m.at(E::e1)  << std::endl;
    std::cout << prefix << ".at(E::e2):  " << m.at(E::e2)  << std::endl;
    std::cout << prefix << ".at(E::e3):  " << m.at(E::e3)  << std::endl;
    std::cout << prefix << ".at(E::e4):  " << m.at(E::e4)  << std::endl;
    std::cout << prefix << ".at(E::e5):  " << m.at(E::e5)  << std::endl;
    std::cout << prefix << ".at(E::e6):  " << m.at(E::e6)  << std::endl;
    std::cout << prefix << ".at(E::e7):  " << m.at(E::e7)  << std::endl;
    std::cout << prefix << ".at(E::e8):  " << m.at(E::e8)  << std::endl;
    std::cout << prefix << ".at(E::e9):  " << m.at(E::e9)  << std::endl;
    std::cout << prefix << ".at(E::e10): " << m.at(E::e10) << std::endl;
}

template<typename T>
void erase_map_by_key(std::string prefix, T& m)
{
    std::cout << ">>> Erase map by keys" << std::endl;
    std::cout << prefix << ".erase(E::e1) : " << m.erase(E::e1)  << std::endl;
    std::cout << prefix << ".erase(E::e2) : " << m.erase(E::e2)  << std::endl;
    std::cout << prefix << ".erase(E::e3) : " << m.erase(E::e3)  << std::endl;
    std::cout << prefix << ".erase(E::e4) : " << m.erase(E::e4)  << std::endl;
    std::cout << prefix << ".erase(E::e5) : " << m.erase(E::e5)  << std::endl;
    std::cout << prefix << ".erase(E::e6) : " << m.erase(E::e6)  << std::endl;
    std::cout << prefix << ".erase(E::e7) : " << m.erase(E::e7)  << std::endl;
    std::cout << prefix << ".erase(E::e8) : " << m.erase(E::e8)  << std::endl;
    std::cout << prefix << ".erase(E::e9) : " << m.erase(E::e9)  << std::endl;
    std::cout << prefix << ".erase(E::e10): " << m.erase(E::e10) << std::endl;

    dump_map(prefix, m);
}

template<typename T>
void erase_map_by_irange(std::string prefix, T& m)
{
    std::cout << ">>> Erase map by iterator range" << std::endl;

    m.erase(m.begin(), m.end());

    dump_map(prefix, m);
}

template<typename T>
void erase_map_by_iterator_increment(std::string prefix, T& m)
{
    std::cout << ">>> Erase map by iterator (increment)" << std::endl;
    typename T::const_iterator it = m.begin();
                   std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;
    m.erase(it++); std::cout << it->first << ":" << it->second << std::endl;

    dump_map(prefix, m);
}

template<typename T>
void erase_map_by_iterator_assign_return(std::string prefix, T& m)
{
    std::cout << ">>> Erase map by iterator (assign return)" << std::endl;
    typename T::const_iterator it = m.begin();
                      std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;
    it = m.erase(it); std::cout << it->first << ":" << it->second << std::endl;

    dump_map(prefix, m);
}

int main(int, char**, char**)
{
    dump_map("m0", m0);
//    erase_map_by_key("m0", m0); // Will emit compile error.

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

    dump_map("m1", m1);
    erase_map_by_key("m1", m1);

    fill_map_ilist("m1", m1);
    erase_map_by_irange("m1", m1);

    fill_map_operator("m1", m1);
    erase_map_by_iterator_increment("m1", m1);

    fill_map_operator("m1", m1);
    erase_map_by_iterator_assign_return("m1", m1);

    return 0;
}
