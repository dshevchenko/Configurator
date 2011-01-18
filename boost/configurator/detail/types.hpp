// detail/types.hpp
// ~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_TYPES_HPP
#define BOOST_CONFIGURATOR_TYPES_HPP

#include <string>
#include <vector>
#include <set>
#include <sstream>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

typedef std::string::iterator       string_it;
typedef std::string::const_iterator string_const_it;

typedef std::vector< std::string >  str_storage;
typedef str_storage::iterator       str_it;
typedef str_storage::const_iterator str_const_it;
typedef std::vector< str_it >       iterators_to_strings;

typedef std::set< std::string >     str_set;
typedef str_set::iterator           str_unique_it;
typedef str_set::const_iterator     str_unique_const_it;

typedef std::ostringstream          o_stream;

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_TYPES_HPP
