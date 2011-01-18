// macro.hpp
// ~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_MACRO_HPP
#define BOOST_CONFIGURATOR_MACRO_HPP

#include <boost/configurator/option.hpp>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

#define BOOST_CONFIGURATOR_OPTION( type )                               \
    struct type : public boost::cf::option {                            \
        type() : boost::cf::option( typeid( type ).name(), #type ) {}   \
    };

} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_MACRO_HPP
