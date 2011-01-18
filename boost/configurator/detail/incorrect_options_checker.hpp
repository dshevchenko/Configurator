// detail/incorrect_options_checker.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_INCORRECT_OPTIONS_CHECKER_HPP
#define BOOST_CONFIGURATOR_INCORRECT_OPTIONS_CHECKER_HPP

#include <boost/configurator/option.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/assign.hpp>

#include <algorithm>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

using namespace boost::assign;

/// \class incorrect_options_checker
/// \brief Incorrect options checker.
///
/// Checks existence of incorrect (unregistered) options.
class incorrect_options_checker {
public:
    incorrect_options_checker( const options& _registered_options
                               , const std::string& _sections_separator ) :
            registered_options( _registered_options )
            , sections_separator( _sections_separator ) {}
private:
    const options& registered_options;
    const std::string& sections_separator;
public:
    void operator()( const pure_options& factual_obtained_options ) {
        BOOST_FOREACH ( const pure_option& option, factual_obtained_options ) {
            option_const_it it = boost::find( registered_options, option.location );
            if ( registered_options.end() == it ) {
                store_incorrect_option( option.location );
            } else {}
        }

        notify_about_incorrect_options_if_such_exists();
    }
private:
    str_storage incorrect_options;
private:
    void store_incorrect_option( const std::string& option_location ) {
        incorrect_options += option_location;
    }

    void notify_about_incorrect_options_if_such_exists() const {
        if ( incorrect_options.empty() ) {
            return;
        } else {}
        
        std::string what_happened = "Incorrect option detected in configuration file:\n"; 
        if ( incorrect_options.size() > 1 ) {
            boost::replace_all( what_happened, "option", "options" );
        } else {}
        BOOST_FOREACH ( const std::string& option, incorrect_options ) {
            what_happened += "'" + prepare_full_name_for_log( option, sections_separator ) + "'\n";
        }
        boost::erase_last( what_happened, "\n" );
        notify( what_happened );
    }
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_INCORRECT_OPTIONS_CHECKER_HPP
