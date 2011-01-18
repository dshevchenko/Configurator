// detail/misc.hpp
// ~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_MISC_HPP
#define BOOST_CONFIGURATOR_MISC_HPP

#include <boost/configurator/detail/types.hpp>
#include <boost/mem_fn.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/range/algorithm_ext.hpp>

#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

inline void notify( const std::string& notification ) {
    const std::string what_happened = "[Configurator] " + notification;
    throw std::runtime_error( what_happened );
}

inline void warning( const std::string& warning_message ) {
    std::cout << "[Configurator] " << warning_message << std::endl;
}

inline std::string sections_separator_for_log() { return " > "; }

inline void notify_without_prefix( const std::string& notification ) {
    throw std::runtime_error( notification );
}

inline void pure_check_path_existence( const std::string& _path ) {
    boost::filesystem::path path( _path );
    
    const std::string full_path_name = path.string();
    const std::string path_name      = path.parent_path().string();
    const std::string file_name      = path.filename();

    if ( boost::filesystem::exists( path_name ) ) {
        if ( !boost::filesystem::exists( full_path_name ) ) {
            notify_without_prefix( "directory '" + path_name
                                   + "' exists, but there is no file '" + file_name + "'!" );
        } else {}
    } else {
        notify_without_prefix( "directory '" + path_name + "' not exists!" );
    }	
}

inline void check_is_it_file( const std::string& _path ) {
    boost::filesystem::path path( _path );
    if ( !boost::filesystem::is_regular_file( path ) ) {
        notify_without_prefix( "path '" + _path + "' exists, but it is not a regular file!" );
    } else {}
}

inline std::string prepare_full_name_for_log( const std::string& full_name_of_option
                                              , const std::string& sections_separator ) {
    std::string s( full_name_of_option.begin(), full_name_of_option.end() );
    boost::replace_all( s, sections_separator, sections_separator_for_log() );
    string_const_it last_separator_end = boost::find_last( s, sections_separator_for_log() ).end();
    if ( s.end() == last_separator_end ) {
        boost::erase_last( s, sections_separator_for_log() );
    } else {}
    return s;
}

inline void remove_empty_strings_from( str_storage& strings ) {
    boost::remove_erase_if( strings, boost::mem_fn( &std::string::empty ) );
}

inline void trim_all( str_storage& storage ) {
    BOOST_FOREACH ( std::string& s, storage ) {
        boost::trim( s );
    }
}

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_MISC_HPP
