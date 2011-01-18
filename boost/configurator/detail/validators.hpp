// detail/validators.hpp
// ~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_VALIDATORS_HPP
#define BOOST_CONFIGURATOR_VALIDATORS_HPP

#include <boost/configurator/option.hpp>
#include <boost/configurator/detail/misc.hpp>

#ifdef WITH_SEMANTIC_CHECK
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/lexical_cast.hpp>

#include <cmath>
#endif

#include <boost/assign.hpp>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

using namespace boost::assign;

inline std::string semantic_error_prefix() { return "Semantic error: "; }

inline void check_ipv4_validity( const std::string& address, const std::string& option_name ) {
    #ifdef WITH_SEMANTIC_CHECK
    try {
        boost::asio::ip::address_v4 a( boost::asio::ip::address_v4::from_string( address ) );
    } catch ( const std::exception& /* exc */ ) {
        notify( semantic_error_prefix()
                + "option '" + option_name + "' has invalid 'IPv4' value '" + address + "'!" );
    }
    #endif
}

inline void check_ipv6_validity( const std::string& address, const std::string& option_name ) {
    #ifdef WITH_SEMANTIC_CHECK
    try {
        boost::asio::ip::address_v6 a( boost::asio::ip::address_v6::from_string( address ) );
    } catch ( const std::exception& /* exc */ ) {
        notify( semantic_error_prefix()
                + "option '" + option_name + "' has invalid 'IPv6' value '" + address + "'!" );
    }
    #endif
}

inline void check_ip_validity( const std::string& address, const std::string& option_name ) {
    #ifdef WITH_SEMANTIC_CHECK
    try {
        check_ipv4_validity( address, option_name );
    } catch ( const std::exception& /* exc */ ) {
        try {
            check_ipv6_validity( address, option_name );
        } catch ( const std::exception& /* exc */ ) {
            notify( semantic_error_prefix()
                    + "option '" + option_name + "' has invalid value '" + address + "' (not IPv4, not IPv6)!" );
        }
    }
    #endif
}

inline void check_path_existence( const std::string& path, const std::string& option_name ) {
    #ifdef WITH_SEMANTIC_CHECK
    try {
        pure_check_path_existence( path );
    } catch ( const std::exception& exc ) {
        notify( semantic_error_prefix()
                + "option '" + option_name + "' has invalid value '" + path + "': " + exc.what() );
    }
    #endif
}

inline void check_optional_path_existence( const std::string& path, const std::string& option_name ) {
    #ifdef WITH_SEMANTIC_CHECK
    try {
        pure_check_path_existence( path );
    } catch ( const std::exception& exc ) {
        warning( "Warning: option '" + option_name
                 + "' has invalid value: '" + path + "': " + exc.what() );
    }
    #endif
}

inline void check_email_validity( const std::string& email, const std::string& option_name ) {
    #ifdef WITH_SEMANTIC_CHECK
    o_stream regex_for_email_validity;
    regex_for_email_validity << "[a-zA-Z0-9_]{1,}"
                             << "[a-zA-Z0-9_.-]{1,}"
                             << "[a-zA-Z0-9_]{1,}"
                             << "@"
                             << "[a-zA-Z0-9_]{1,}"
                             << "[a-zA-Z0-9_.-]{1,}"
                             << "[.]{1}"
                             << "[a-zA-Z]{2,6}"
                             ;
    const boost::regex e( regex_for_email_validity.str() );
    if ( !regex_match( email, e ) ) {
        notify( semantic_error_prefix()
                + "option '" + option_name + "' has invalid 'E-MAIL' value '" + email + "'!" );
    } else {} 
    #endif
}

///
struct common_validator {
#ifdef WITH_SEMANTIC_CHECK
protected:
    typedef boost::function< bool ( const std::string& /* string_with_value */ ) >
            common_checker;
    typedef boost::function< void ( const std::string& /* string_with_value */
                                    , const std::string& /* option_name */ ) >
            common_storer;
    typedef std::pair
                <
                    common_checker
                    , common_storer
                >
            check_pair;
    typedef std::vector< check_pair >
            common_handlers;
#endif
protected:
    common_validator( options& _registered_options, const std::string& _sections_separator ) 
            #ifdef WITH_SEMANTIC_CHECK
            : registered_options( _registered_options )
            , sections_separator( _sections_separator ) 
            #endif
    {}
#ifdef WITH_SEMANTIC_CHECK
protected:
    options& registered_options;
    const std::string& sections_separator;
    common_handlers handlers;
protected:
    void check( const std::string&   what
                , const std::string& checked_string_with_value
                , const std::string& option_name ) {
        check_negitivity_of( what, checked_string_with_value, option_name );  
        bool coincided = false;
        BOOST_FOREACH ( check_pair& p, handlers ) {
            if ( p.first( checked_string_with_value ) ) {
                p.second( checked_string_with_value, option_name );
                coincided = true;
            } else {}
        }
        if ( !coincided ) {
            notify( semantic_error_prefix()
                    + "option '" + option_name
                    + "' has invalid '" + what + "' value '" + checked_string_with_value + "'!" );
        } else {}
    }

    void check_negitivity_of( const std::string&   what
                              , const std::string& string_with_value
                              , const std::string& option_name ) const {
        if ( boost::starts_with( string_with_value, "-" ) ) {
            notify( semantic_error_prefix()
                    + "option '" + option_name
                    + "' has invalid '" + what + "' value '" + string_with_value
                    + "', it cannot be negative!" );
        } else {}
    }

    double extract_pure_value_from( const std::string& string_with_value, const std::string& value_sign ) const {
        string_const_it value_begin = string_with_value.begin();
        string_const_it value_end   = boost::ifind_last( string_with_value, value_sign ).begin();
        std::string pure_value_as_string( value_begin, value_end );
        boost::trim( pure_value_as_string );
        return boost::lexical_cast< double >( pure_value_as_string );
    }

    template< typename ValueType >
    void store_for_option( const std::string& option_name, ValueType value_in_minimal_units ) {
        std::string restored_option_name( option_name.begin(), option_name.end() );
        boost::replace_all( restored_option_name, sections_separator_for_log(), sections_separator );
        option_it it = boost::range::find( registered_options, restored_option_name );
        if ( registered_options.end() != it ) {
            it->value = boost::lexical_cast< std::string >( value_in_minimal_units );
        } else {}
    }
#endif
};

/// \class size_validator
/// \brief Size validator.
///
/// Check size semantic.
class size_validator : common_validator {
public:
    size_validator( options& registered_options, const std::string& sections_separator ) :
            common_validator( registered_options, sections_separator )
            #ifdef WITH_SEMANTIC_CHECK
            , regex_size_in_bytes_pure( "[0-9]{1,}" )
            , regex_size_in_bytes( "[0-9]{1,}[ ]{0,}[bB]{1}" )
            , regex_size_in_kbytes( "[0-9.]{1,}[ ]{0,}[kK]{1}[bB]{0,1}" )
            , regex_size_in_mbytes( "[0-9.]{1,}[ ]{0,}[mM]{1}[bB]{0,1}" )
            , regex_size_in_gbytes( "[0-9.]{1,}[ ]{0,}[gG]{1}[bB]{0,1}" )
            , b_sign( "b" )
            , kb_sign( "k" )
            , mb_sign( "m" )
            , gb_sign( "g" )
            , bytes_in_kb( ::pow( 1024, 1 ) )
            , bytes_in_mb( ::pow( 1024, 2 ) ) 
            , bytes_in_gb( ::pow( 1024, 3 ) ) 
            #endif
    {
        #ifdef WITH_SEMANTIC_CHECK
        typedef size_validator sv;
        handlers = list_of( std::make_pair( boost::bind( &sv::size_in_bytes_pure, this, ::_1 )
                                            , boost::bind( &sv::store_in_bytes_pure, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &sv::size_in_bytes, this, ::_1 )
                                            , boost::bind( &sv::store_in_bytes, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &sv::size_in_kbytes, this, ::_1 )
                                            , boost::bind( &sv::store_in_kbytes, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &sv::size_in_mbytes, this, ::_1 )
                                            , boost::bind( &sv::store_in_mbytes, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &sv::size_in_gbytes, this, ::_1 )
                                            , boost::bind( &sv::store_in_gbytes, this, ::_1, ::_2 ) ) )
        ;
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    const boost::regex regex_size_in_bytes_pure;
    const boost::regex regex_size_in_bytes;
    const boost::regex regex_size_in_kbytes;
    const boost::regex regex_size_in_mbytes;
    const boost::regex regex_size_in_gbytes;
    const std::string b_sign;
    const std::string kb_sign;
    const std::string mb_sign;
    const std::string gb_sign;
    const unsigned long int bytes_in_kb;
    const unsigned long int bytes_in_mb;
    const unsigned long int bytes_in_gb;
#endif
public:
    void check( const std::string& size, const std::string& option_name ) {
        #ifdef WITH_SEMANTIC_CHECK
        common_validator::check( "SIZE", size, option_name );
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    bool size_in_bytes_pure( const std::string& size ) const { return regex_match( size, regex_size_in_bytes_pure ); }
    bool size_in_bytes( const std::string& size ) const      { return regex_match( size, regex_size_in_bytes ); }
    bool size_in_kbytes( const std::string& size ) const     { return regex_match( size, regex_size_in_kbytes ); }
    bool size_in_mbytes( const std::string& size ) const     { return regex_match( size, regex_size_in_mbytes ); }
    bool size_in_gbytes( const std::string& size ) const     { return regex_match( size, regex_size_in_gbytes ); } 
private:
    void store_in_bytes_pure( const std::string& size, const std::string& option_name ) {
        const double value_in_bytes = boost::lexical_cast< double >( size );
        store_for_option( option_name, value_in_bytes );
    }
    
    void store_in_bytes( const std::string& size, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( size, b_sign ); 
        const unsigned long int value_in_bytes = pure_value;
        store_for_option( option_name, value_in_bytes );
    }

    void store_in_kbytes( const std::string& size, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( size, kb_sign ); 
        const unsigned long int value_in_bytes = pure_value * bytes_in_kb;
        store_for_option( option_name, value_in_bytes );
    }

    void store_in_mbytes( const std::string& size, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( size, mb_sign ); 
        const unsigned long int value_in_bytes = pure_value * bytes_in_mb;
        store_for_option( option_name, value_in_bytes );
    }

    void store_in_gbytes( const std::string& size, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( size, gb_sign ); 
        const unsigned long int value_in_bytes = pure_value * bytes_in_gb;
        store_for_option( option_name, value_in_bytes );
    } 
#endif
};

/// \class time_period_validator
/// \brief Time period validator.
///
/// Check time period semantic.
class time_period_validator : common_validator { 
public:
    time_period_validator( options& registered_options, const std::string& sections_separator ) :
            common_validator( registered_options, sections_separator )
            #ifdef WITH_SEMANTIC_CHECK
            , second_sign( "s" )
            , minute_sign( "m" )
            , hour_sign( "h" )
            , day_sign( "d" )
            , regex_period_in_seconds_pure( "[0-9]{1,}" )
            , regex_period_in_seconds( "[0-9]{1,}[ ]{0,}[" + second_sign + "]{1}" )
            , regex_period_in_minutes( "[0-9.]{1,}[ ]{0,}[" + minute_sign + "]{1}" )
            , regex_period_in_hours( "[0-9.]{1,}[ ]{0,}[" + hour_sign + "]{1}" )
            , regex_period_in_days( "[0-9.]{1,}[ ]{0,}[" + day_sign + "]{1}" )
            , seconds_in_minute( 60 )
            , seconds_in_hour( 60 * seconds_in_minute ) 
            , seconds_in_day( 24 * seconds_in_hour ) 
            #endif
    {
        #ifdef WITH_SEMANTIC_CHECK
        typedef time_period_validator tpv; 
        handlers = list_of( std::make_pair( boost::bind( &tpv::period_in_seconds_pure, this, ::_1 )
                                            , boost::bind( &tpv::store_in_seconds_pure, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &tpv::period_in_seconds, this, ::_1 )
                                            , boost::bind( &tpv::store_in_seconds, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &tpv::period_in_minutes, this, ::_1 )
                                            , boost::bind( &tpv::store_in_minutes, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &tpv::period_in_hours, this, ::_1 )
                                           , boost::bind( &tpv::store_in_hours, this, ::_1, ::_2 ) ) )
                          ( std::make_pair( boost::bind( &tpv::period_in_days, this, ::_1 )
                                            , boost::bind( &tpv::store_in_days, this, ::_1, ::_2 ) ) )
                          ; 
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    const std::string second_sign;
    const std::string minute_sign;
    const std::string hour_sign;
    const std::string day_sign;
    const boost::regex regex_period_in_seconds_pure;
    const boost::regex regex_period_in_seconds;
    const boost::regex regex_period_in_minutes;
    const boost::regex regex_period_in_hours;
    const boost::regex regex_period_in_days;
    const unsigned long int seconds_in_minute;
    const unsigned long int seconds_in_hour;
    const unsigned long int seconds_in_day;
#endif
public:
    void check( const std::string& time_period, const std::string& option_name ) {
        #ifdef WITH_SEMANTIC_CHECK
        common_validator::check( "TIME PERIOD", time_period, option_name );
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    bool period_in_seconds_pure( const std::string& time_period ) const {
        return regex_match( time_period, regex_period_in_seconds_pure );
    }
    
    bool period_in_seconds( const std::string& time_period ) const {
        return regex_match( time_period, regex_period_in_seconds );
    }
    
    bool period_in_minutes( const std::string& time_period ) const {
        return regex_match( time_period, regex_period_in_minutes );
    }
    
    bool period_in_hours( const std::string& time_period ) const {
        return regex_match( time_period, regex_period_in_hours );
    }
    
    bool period_in_days( const std::string& time_period ) const {
        return regex_match( time_period, regex_period_in_days );
    } 
private:
    void store_in_seconds_pure( const std::string& time_period, const std::string& option_name ) {
        const double value_in_seconds = boost::lexical_cast< double >( time_period );
        store_for_option( option_name, value_in_seconds );
    }
    
    void store_in_seconds( const std::string& time_period, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( time_period, second_sign ); 
        const unsigned long int value_in_seconds = pure_value;
        store_for_option( option_name, value_in_seconds );
    }

    void store_in_minutes( const std::string& time_period, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( time_period, minute_sign ); 
        const unsigned long int value_in_seconds = pure_value * seconds_in_minute;
        store_for_option( option_name, value_in_seconds );
    }

    void store_in_hours( const std::string& time_period, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( time_period, hour_sign ); 
        const unsigned long int value_in_seconds = pure_value * seconds_in_hour;
        store_for_option( option_name, value_in_seconds );
    }

    void store_in_days( const std::string& time_period, const std::string& option_name ) {
        const double pure_value = extract_pure_value_from( time_period, day_sign ); 
        const unsigned long int value_in_seconds = pure_value * seconds_in_day;
        store_for_option( option_name, value_in_seconds );
    }
#endif
};

/// \class exp_record_validator
/// \brief Exponential record validator.
///
/// Check exponential record semantics.
class exp_record_validator : common_validator { 
public:
    exp_record_validator( options& registered_options, const std::string& sections_separator ) :
            common_validator( registered_options, sections_separator ) {}
public:
    void check( const std::string& exp_record, const std::string& option_name ) {
        #ifdef WITH_SEMANTIC_CHECK
        using boost::spirit::qi::long_double;
        using boost::spirit::qi::print;
        using boost::spirit::qi::parse;
    
        double number = 0.0;
        bool valid_exp_record = parse( exp_record.begin()
                                       , exp_record.end()
                                       , long_double >> !print
                                       , number );
        if ( valid_exp_record ) {
            store_for_option( option_name, number );
        } else {
            notify_about_invalid_exp_record( option_name, exp_record );
        } 
        #endif
    }
#ifdef WITH_SEMANTIC_CHECK
private:
    void notify_about_invalid_exp_record( const std::string& option_name
                                          , const std::string& exp_record ) const {
        notify( semantic_error_prefix() + "option '" + option_name
                + "' has invalid 'EXP_RECORD' value '" + exp_record + "'!" );
    }
#endif
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_VALIDATORS_HPP
