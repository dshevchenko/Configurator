// detail/pure_options_obtainer.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_PURE_OPTIONS_OBTAINER_HPP
#define BOOST_CONFIGURATOR_PURE_OPTIONS_OBTAINER_HPP

#include <boost/configurator/detail/types.hpp>
#include <boost/configurator/detail/misc.hpp>
#include <boost/configurator/detail/pure_option.hpp>
#include <boost/configurator/detail/grammars.hpp>
#include <boost/configurator/option.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <string>
#include <vector>
#include <algorithm>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

class pure_options_obtainer {
    typedef boost::function< bool ( std::string& /* analized string */
                                    , pure_options& /* factual obtained options */ ) >
            string_handler;
    typedef std::vector< string_handler >
            string_handlers;
    typedef boost::spirit::qi::rule< string_it >
            simple_rule;
public:
    pure_options_obtainer( const char&          _option_name_value_separator
                           , const std::string& _sections_separator
                           , const options&     _registered_options
                           , const bool&        _case_sensitivity_for_names
                           , const std::string& _open_section_tag_begin_sign
                           , const std::string& _open_section_tag_end_sign
                           , const std::string& _close_section_tag_begin_sign
                           , const std::string& _close_section_tag_end_sign) :
            option_name_value_separator( _option_name_value_separator )
            , sections_separator( _sections_separator )
            , registered_options( _registered_options )
            , case_sensitivity_for_names( _case_sensitivity_for_names ) 
            , open_section_tag_begin_sign( _open_section_tag_begin_sign )
            , open_section_tag_end_sign( _open_section_tag_end_sign )
            , close_section_tag_begin_sign( _close_section_tag_begin_sign )
            , close_section_tag_end_sign( _close_section_tag_end_sign ) {
        using namespace boost::assign;
        handlers +=   boost::bind( &pure_options_obtainer::handle_section_opening, this, ::_1, ::_2 )
                    , boost::bind( &pure_options_obtainer::handle_section_closing, this, ::_1, ::_2 )
                    , boost::bind( &pure_options_obtainer::handle_option,          this, ::_1, ::_2 )
                    , boost::bind( &pure_options_obtainer::meaningless_string,     this, ::_1, ::_2 )
                    ;
    }
private:
    const char&         option_name_value_separator;
    const std::string&  sections_separator;
    const options&      registered_options;
    const bool&         case_sensitivity_for_names;
private:
    const std::string&  open_section_tag_begin_sign;
    const std::string&  open_section_tag_end_sign;
    const std::string&  close_section_tag_begin_sign;
    const std::string&  close_section_tag_end_sign;
private:
    std::string current_section_path;
private:
    string_handlers handlers;
public:
    pure_options operator()( str_storage& obtained_strings ) {
        pure_options factual_obtained_options;
        BOOST_FOREACH ( std::string& s, obtained_strings ) {
            handle( s, factual_obtained_options ); 
        }
        check_last_section_closing();
        return factual_obtained_options;
    }
private:
    void handle( std::string& s, pure_options& factual_obtained_options ) {
        BOOST_FOREACH ( string_handler& handler, handlers ) {
            const bool handling_success = handler( s, factual_obtained_options );
            if ( handling_success ) {
                break;
            } else {}
        }
    }
private:
    bool handle_section_opening( std::string& s, pure_options& /* factual_obtained_options */ ) {
        using boost::spirit::qi::_1;
        using boost::phoenix::push_back;
        using boost::spirit::qi::string;
        using boost::spirit::qi::char_;

        std::string name_of_opening_section;

        simple_rule open_edge = !string( close_section_tag_begin_sign ) 
                                >> string( open_section_tag_begin_sign );
        simple_rule name_extractor = +( char_[ push_back( boost::phoenix::ref(name_of_opening_section), _1 ) ]
                                        - string( open_section_tag_end_sign ) );
        simple_rule close_edge = string( open_section_tag_end_sign );
        simple_rule full = open_edge >> name_extractor >> close_edge;

        bool parsing_success = parse( s.begin(), s.end(), full );
        if ( parsing_success ) {
            boost::trim( name_of_opening_section );
            final_handle_section_opening( name_of_opening_section );
        } else {}
        return parsing_success;
    }
    
    void final_handle_section_opening( std::string& name_of_opening_section ) {
        convert_name_depending_on_case_sensitivity( name_of_opening_section );
        check_duplication_of( name_of_opening_section );
        current_section_path += name_of_opening_section + sections_separator;
    }
    
    void check_duplication_of( const std::string& opening_section_name ) const {
        const std::string name_of_last_opening_section = retrieve_name_of_last_opening_section();
        if ( opening_section_name == name_of_last_opening_section ) {
            notify( "Duplication of open tag for section '"
                    + prepare_full_name_for_log( current_section_path, sections_separator )
                    + "' detected!" );
        } else {}
    }

    std::string retrieve_name_of_last_opening_section() const {
        string_const_it name_end = boost::find_last( current_section_path, sections_separator ).begin();
        string_const_it name_begin;
        string_const_it checker = std::search( current_section_path.begin()
                                               , name_end
                                               , sections_separator.begin()
                                               , sections_separator.end() );
        if ( name_end != checker ) {
            name_begin = boost::find_nth( current_section_path, sections_separator, -2 ).end();
        } else {
            name_begin = current_section_path.begin();
        }
        return std::string( name_begin, name_end );
    }
private:
    bool handle_section_closing( std::string& s, pure_options& /* factual_obtained_options */ ) {
        using boost::spirit::qi::_1;
        using boost::phoenix::push_back;
        using boost::spirit::qi::string;
        using boost::spirit::qi::char_;

        std::string name_of_closing_section;

        simple_rule open_edge = string( close_section_tag_begin_sign ); 
        simple_rule name_extractor = +( char_[ push_back( boost::phoenix::ref(name_of_closing_section), _1 ) ] 
                                        - string( close_section_tag_end_sign ) );
        simple_rule close_edge = string( close_section_tag_end_sign );
        simple_rule full = open_edge >> name_extractor >> close_edge;

        bool parsing_success = parse( s.begin(), s.end(), full );
        if ( parsing_success ) {
            boost::trim( name_of_closing_section );
            final_handle_section_closing( name_of_closing_section );
        } else {}
        return parsing_success;
    }
    
    void final_handle_section_closing( std::string& name_of_closing_section ) {
        convert_name_depending_on_case_sensitivity( name_of_closing_section );
        check_is_any_section_was_opened( name_of_closing_section );
        check_section_symmetry( name_of_closing_section );
        cut_current_section_name_by( name_of_closing_section );
    }

    void check_is_any_section_was_opened( const std::string& closing_section_name ) const {
        if ( current_section_path.empty() ) {
            notify( "Closing of section '" + closing_section_name + "' detected, "
                    + "but this section was not opened!" );
        } else {}
    }

    void check_section_symmetry( const std::string& closing_section_name ) const {
        const std::string name_of_last_opening_section = retrieve_name_of_last_opening_section();
        if ( closing_section_name != name_of_last_opening_section ) {
            notify( "Dissymmetry of sections detected: last opening section '"
                    + name_of_last_opening_section + "', but closing is '" + closing_section_name + "'!" );
        } else {}
    }
    
    void cut_current_section_name_by( const std::string& closing_section_name ) {
        boost::erase_last( current_section_path, closing_section_name + sections_separator );
    }
private:
    bool handle_option( std::string& s, pure_options& factual_obtained_options ) {
        bool parsing_success = false;

        obtained_option result;
        if ( ' ' == option_name_value_separator ) {
            parsing_success = parse_option_space( s, result );
        } else {
            parsing_success = parse_option( s, option_name_value_separator, result );
        }

        if ( parsing_success ) {
            boost::trim( result.name );
            boost::trim( result.value );
            final_handle_option( result.name, result.value, factual_obtained_options );
        } else {}

        return parsing_success; 
    }

    void final_handle_option( const std::string&    option_name
                              , const std::string&  option_value
                              , pure_options&       factual_obtained_options ) {
        const std::string option_location = current_section_path + option_name;
        factual_obtained_options += pure_option( option_location, option_value );
    }
private:
    bool meaningless_string( std::string& s, pure_options& /* factual_obtained_options */ ) const {
        std::string what_happened = "in global scope";
        if ( !current_section_path.empty() ) {
            what_happened = "in section '"
                            + prepare_full_name_for_log( current_section_path, sections_separator )
                            + "'"
                            ;
        } else {}
        what_happened = "Meaningless string '" + s + "' detected " + what_happened + "!";
        notify( what_happened );
        return true;
    }
private:
    void check_last_section_closing() {
        if ( !current_section_path.empty() ) {
            boost::erase_last( current_section_path, sections_separator );
            notify( "Section '" + current_section_path + "' unclosed!" );
        } else {}
    }

    void convert_name_depending_on_case_sensitivity( std::string& section_name ) const {
        if ( !case_sensitivity_for_names ) {
            boost::to_lower( section_name );
        } else {}
    } 
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_PURE_OPTIONS_OBTAINER_HPP
