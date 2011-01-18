// detail/pure_option.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_PURE_OPTION_HPP
#define BOOST_CONFIGURATOR_PURE_OPTION_HPP

#include <string>
#include <vector>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \struct pure_option
/// \brief 
///
/// Presents pure option obtained from configuration file.
struct pure_option {
    pure_option() {}
    pure_option( const std::string& _location, const std::string& _value ) :
            location( _location.begin(), _location.end() )
            , value( _value.begin(), _value.end() ) {}
public:
    std::string location;
    std::string value;
public:
    bool empty() const {
        return value.empty();
    }

    bool operator==( const std::string& _location ) const {
        return _location == location;
    }

    bool operator==( const pure_option& another ) const {
        return another.location == location;
    }
};

inline bool operator<( const pure_option& left, const pure_option& right ) {
    return left.location < right.location;
}

typedef std::vector< pure_option >      pure_options;
typedef pure_options::iterator          pure_option_it;
typedef pure_options::const_iterator    pure_option_const_it;

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_PURE_OPTION_HPP
