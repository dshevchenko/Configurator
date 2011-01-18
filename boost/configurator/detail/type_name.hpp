// detail/type_name.hpp
// ~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_TYPE_NAME_HPP
#define BOOST_CONFIGURATOR_TYPE_NAME_HPP

#include <boost/assign.hpp>

#include <map>
#include <string>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

class type_name {
private:
    typedef std::map
                <
                    const std::string   // typeid.name()
                    , const std::string // type name
                >
            type_names;
public:
    type_name() {
        using namespace boost::assign;
        insert( names )( typeid( bool ).name(),                  "bool" )
                       ( typeid( char ).name(),                  "char" )
                       ( typeid( signed char ).name(),           "signed char" )
                       ( typeid( unsigned char ).name(),         "unsigned char" )
                       ( typeid( int ).name(),                   "int" )
                       ( typeid( signed int ).name(),            "signed int" )
                       ( typeid( unsigned int ).name(),          "unsigned int" )
                       ( typeid( short int ).name(),             "short int" )
                       ( typeid( long int ).name(),              "long int" )
                       ( typeid( unsigned short int ).name(),    "unsigned short int" )
                       ( typeid( unsigned long int ).name(),     "unsigned long int" )
                       ( typeid( signed short int ).name(),      "signed short int" )
                       ( typeid( signed long int ).name(),       "signed long int" )
                       ( typeid( float ).name(),                 "float" )
                       ( typeid( double ).name(),                "double" )
                       ( typeid( long double ).name(),           "long double" )
                       ( typeid( std::string ).name(),           "std::string" )
                       ;
    }
private:
    type_names names;
public:
    template< typename Type >
    std::string name_of() {
        std::string type_name = "<unknown>";
        try {
            type_name = "<" + names.at( typeid( Type ).name() ) + ">";
        } catch ( const std::exception& /* exc */ ) {}
        return type_name;
    }
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_TYPE_NAME_HPP
