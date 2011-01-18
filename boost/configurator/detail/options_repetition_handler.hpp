// detail/options_repetition_handler.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_OPTIONS_REPETITION_HANDLER_HPP
#define BOOST_CONFIGURATOR_OPTIONS_REPETITION_HANDLER_HPP

#include <boost/configurator/detail/types.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/algorithm.hpp>

// #include <algorithm>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

namespace fs = boost::filesystem;

/// \class options_repetition_handler
/// \brief Options repetition handler.
///
/// Handle repetition of factual obtained option,
/// with corresponding checks.
class options_repetition_handler {
public:
    options_repetition_handler( const options&       _registered_options
                                , const std::string& _sections_separator ) :
            registered_options( _registered_options )
            , sections_separator( _sections_separator ) {}
private:
    const options& registered_options;
    const std::string& sections_separator;
public:
    void operator()( pure_options& factual_obtained_options ) const {
        boost::sort( factual_obtained_options );
        pure_options unique_options = retrieve_unique_options_from( factual_obtained_options );

        if ( no_repetition( unique_options, factual_obtained_options ) ) {
            return;
        } else {}

        BOOST_FOREACH ( const pure_option& unique_option, unique_options ) {
            pure_option_it first_repeating = boost::range::find( factual_obtained_options, unique_option.location );
            if ( factual_obtained_options.end() != first_repeating ) {
                size_t how_many_times_it_repeats = (size_t)boost::count( factual_obtained_options
                                                                         , unique_option.location );
                if ( 1 == how_many_times_it_repeats ) {
                    continue;
                } else {}

                check_multi_values_allowance( *first_repeating );

                std::string values_buffer;

                for ( pure_option_it it = first_repeating;
                      it != first_repeating + how_many_times_it_repeats;
                      ++it ) {
                    values_buffer += it->value + " ";
                    it->value.clear();
                }

                first_repeating->value = values_buffer;
            } else {}
        }
        
        remove_empty_options_from( factual_obtained_options );
    }
private:
    pure_options retrieve_unique_options_from( const pure_options& factual_obtained_options ) const {
        pure_options unique_options( factual_obtained_options.size() );
        pure_option_it repeating_end = boost::unique_copy( factual_obtained_options
                                                           , unique_options.begin() );
        unique_options.erase( repeating_end, unique_options.end() );
        return unique_options;
    }
    
    bool no_repetition( const pure_options& unique_options, const pure_options& factual_obtained_options ) const {
        return unique_options.size() == factual_obtained_options.size();
    }

    void check_multi_values_allowance( const pure_option& factual_obtained_option ) const {
        option_const_it it = boost::find( registered_options, factual_obtained_option.location );
        if ( registered_options.end() != it ) {
            if ( !it->multi_values_allowed ) {
                notify( "Option '" + prepare_full_name_for_log( factual_obtained_option.location
                                                                , sections_separator )
                        + "' has multiple values, but it not allowed to have multiply values!" );
            } else {}
        } else {}
    } 

    void remove_empty_options_from( pure_options& factual_obtained_options ) const {
        boost::remove_erase_if( factual_obtained_options, boost::mem_fn( &pure_option::empty ) );
    }
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_OPTIONS_REPETITION_HANDLER_HPP
