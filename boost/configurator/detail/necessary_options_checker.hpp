// detail/necessary_options_checker.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_NECESSARY_OPTIONS_CHECKER_HPP
#define BOOST_CONFIGURATOR_NECESSARY_OPTIONS_CHECKER_HPP

#include <boost/configurator/option.hpp>
#include <boost/range/algorithm_ext.hpp>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \class necessary_options_checker 
/// \brief Necessary options checker.
///
/// Checks existence of all options defined as necessary.
class necessary_options_checker {
public:
    necessary_options_checker( const options& _registered_options
                               , const std::string& _sections_separator ) :
            registered_options( _registered_options )
            , sections_separator( _sections_separator ) {}
private:
    const options& registered_options;
    const std::string& sections_separator;
public:
    void operator()( const pure_options& factual_obtained_options ) const {
        str_storage names_that_should_be = collect_names_of_necessary_options();  
        remove_names_of_inputed_necessary_options( factual_obtained_options, names_that_should_be );
        if ( !names_that_should_be.empty() ) {
            notify_about_missing_of_necessary_options( names_that_should_be );	
        } else {}
    }
private:
    str_storage collect_names_of_necessary_options() const {
        str_storage names;
    	BOOST_FOREACH ( const option& registered_option, registered_options ) {
            if ( registered_option.is_necessary() ) {
            	names += registered_option.location;
            } else {}
        }
        return names;
    }
    
    void remove_names_of_inputed_necessary_options( const pure_options& factual_obtained_options
                                                    , str_storage&      names_that_should_be ) const {
        BOOST_FOREACH ( const pure_option& obtained_option, factual_obtained_options ) {
            boost::remove_erase( names_that_should_be, obtained_option.location );
        }
    }

    void notify_about_missing_of_necessary_options( const str_storage& names_that_should_be ) const {
        std::string what_happened = "Option ";
        
        BOOST_FOREACH ( const std::string& name, names_that_should_be ) {
        	what_happened += "'" + prepare_full_name_for_log( name, sections_separator ) + "', ";
        }
        boost::erase_last( what_happened, ", " );
        
        if ( 1 == names_that_should_be.size() ) {
            what_happened += " is defined as necessary, but it missed!";
        } else {
            boost::replace_first( what_happened, "Option", "Options" );
            what_happened += " are defined as necessary, but they missed!";
        }

        notify( what_happened );
    }
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_NECESSARY_OPTIONS_CHECKER_HPP
