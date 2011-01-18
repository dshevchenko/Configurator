// detail/semantics_checker.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_SEMANTICS_CHECKER_HPP
#define BOOST_CONFIGURATOR_SEMANTICS_CHECKER_HPP

#include <boost/configurator/option.hpp>

#include <string>

#ifdef WITH_SEMANTIC_CHECK
#include <boost/configurator/detail/validators.hpp>
#include <boost/configurator/detail/value_semantics.hpp>
#include <boost/function.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

#include <map>
#include <algorithm>
#endif

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \class semantics_checker
/// \brief Semantics checker.
///
/// Checks semantics of option's values, if it defined.
class semantics_checker {
    #ifdef WITH_SEMANTIC_CHECK
    typedef boost::function< void ( const std::string& /* arg */
                                    , const std::string& /* option name */ ) >
            check_func;

    typedef std::map
                < 
                    const value_semantic
                    , const check_func
                >
            checkers;
    #endif
public:
    semantics_checker( options& _registered_options
                      , const std::string& _sections_separator )
            #ifdef WITH_SEMANTIC_CHECK
            : registered_options( _registered_options )
            , sections_separator( _sections_separator )
            , check_size_validity( registered_options, sections_separator )
            , check_time_period_validity( registered_options, sections_separator )
            , check_exp_record_validity( registered_options, sections_separator ) 
            #endif
    {
        #ifdef WITH_SEMANTIC_CHECK
        using namespace boost::assign;
        insert( semantic_checkers )( path,          check_path_existence )
                                   ( optional_path, check_optional_path_existence )
                                   ( ipv4,          check_ipv4_validity )
                                   ( ipv6,          check_ipv6_validity )
                                   ( ip,            check_ip_validity )
                                   ( email,         check_email_validity )
                                   ( size,          boost::bind( &size_validator::check
                                                                 , &check_size_validity
                                                                 , ::_1
                                                                 , ::_2 ) )
                                   ( time_period,   boost::bind( &time_period_validator::check
                                                                 , &check_time_period_validity
                                                                 , ::_1
                                                                 , ::_2 ) )
                                   ( exp_record,    boost::bind( &exp_record_validator::check
                                                                 , &check_exp_record_validity
                                                                 , ::_1
                                                                 , ::_2 ) )
                                   ;
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    options&                registered_options;
    const std::string&      sections_separator;
    size_validator          check_size_validity;
    time_period_validator   check_time_period_validity;
    exp_record_validator    check_exp_record_validity;
    checkers                semantic_checkers;
#endif
public:
    void operator()() {
        #ifdef WITH_SEMANTIC_CHECK
        std::for_each( registered_options.begin()
                       , registered_options.end()
                       , boost::bind( &semantics_checker::check, this, ::_1 ) ); 
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    void check( const option& registered_option ) {
        if ( semantics_check_needed( registered_option ) ) {
            const std::string& value = registered_option.value;
            const std::string location = prepare_full_name_for_log( registered_option.location
                                                                    , sections_separator );
            semantic_checkers[ registered_option.semantic ]( value, location );
        } else {}
    }

    bool semantics_check_needed( const option& registered_option ) const {
        return !registered_option.empty() && registered_option.semantic_defined();
    }
#endif
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_SEMANTICS_CHECKER_HPP
