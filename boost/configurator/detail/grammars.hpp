// detail/grammars.hpp
// ~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_GRAMMARS_HPP
#define BOOST_CONFIGURATOR_GRAMMARS_HPP

#include <boost/configurator/detail/types.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <string>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

struct obtained_option {
    std::string name;
    std::string value;
};

} // namespace detail
} // namespace cf
} // namespace boost

BOOST_FUSION_ADAPT_STRUCT(
    boost::cf::detail::obtained_option,
    (std::string, name)
    (std::string, value)
)

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

template< typename Iterator >
struct option_parser_space : boost::spirit::qi::grammar< Iterator
                                                         , obtained_option()
                                                         , boost::spirit::ascii::space_type > {
    typedef typename boost::spirit::qi::rule
            <
                Iterator
                , std::string()
                , boost::spirit::ascii::space_type
            >
        string_rule;
    typedef typename boost::spirit::qi::rule
            < 
                Iterator
                , obtained_option()
                , boost::spirit::ascii::space_type
            >
        base_rule;
public:
    option_parser_space() :
            option_parser_space::base_type( start ) {
        using boost::spirit::qi::lexeme;
        using boost::spirit::qi::char_;
         
        name_extractor  %= lexeme[ +( char_ - ' ' ) ];
        value_extractor %= lexeme[ +( char_ - ' ' ) ];
        start %= name_extractor >> value_extractor;
    }
public: 
    string_rule name_extractor;
    string_rule value_extractor;
    base_rule   start;
};

template< typename Iterator >
struct option_parser : boost::spirit::qi::grammar< Iterator
                                                   , obtained_option()
                                                   , boost::spirit::ascii::space_type > {
    typedef typename boost::spirit::qi::rule
            <
                Iterator
                , std::string()
                , boost::spirit::ascii::space_type
            >
        string_rule;
    typedef typename boost::spirit::qi::rule
            < 
                Iterator
                , obtained_option()
                , boost::spirit::ascii::space_type
            >
        base_rule;
public:
    option_parser( char separator ) :
            option_parser::base_type( start ) {
        using boost::spirit::qi::lexeme;
        using boost::spirit::qi::char_;
         
        name_extractor  %= lexeme[ +( char_ - separator ) ];
        value_extractor %= lexeme[ +( char_ - separator ) ];
        start %= name_extractor >> separator >> value_extractor;
    }
public: 
    string_rule name_extractor;
    string_rule value_extractor;
    base_rule   start;
};

typedef option_parser< string_it >              pure_option_parser;
typedef option_parser< string_const_it >        pure_option_parser_const;

typedef option_parser_space< string_it >        pure_option_parser_space;
typedef option_parser_space< string_const_it >  pure_option_parser_space_const;

inline bool parse_option( std::string& s, char separator, obtained_option& result ) {
    pure_option_parser parser( separator );
    string_it first = s.begin();
    string_it last  = s.end();
    bool parse_success = boost::spirit::qi::phrase_parse( first, last
                                                          , parser
                                                          , boost::spirit::ascii::space
                                                          , result );
    return parse_success && first == last;
}

inline bool parse_option_space( std::string& s, obtained_option& result ) {
    pure_option_parser_space parser;
    string_it first = s.begin();
    string_it last  = s.end();
    bool parse_success = boost::spirit::qi::phrase_parse( first, last
                                                          , parser
                                                          , boost::spirit::ascii::space
                                                          , result );
    return parse_success && first == last;
}

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_GRAMMARS_HPP
