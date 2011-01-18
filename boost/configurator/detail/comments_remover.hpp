// detail/comments_remover.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP
#define BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP

#include <boost/configurator/detail/misc.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <algorithm>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \class comments_remover
/// \brief Comments remover.
///
/// Removes one-line comments and multi-line comments.

class comments_remover {
    typedef boost::spirit::qi::rule< string_const_it > simple_rule;
public:
    comments_remover( const std::string&   _one_line_comment_sign
                      , const std::string& _multi_line_comment_begin_sign
                      , const std::string& _multi_line_comment_end_sign) :
            one_line_comment_sign( _one_line_comment_sign )
            , multi_line_comment_begin_sign( _multi_line_comment_begin_sign )
            , multi_line_comment_end_sign( _multi_line_comment_end_sign ) {}
private:
    const std::string& one_line_comment_sign;
    const std::string& multi_line_comment_begin_sign;
    const std::string& multi_line_comment_end_sign;
public:
    void operator()( str_storage& obtained_strings ) const {
        std::string obtained = concatenate( obtained_strings );
        std::string without_one_line_comments;
        bool remove_success = remove_one_line_comments( obtained, without_one_line_comments );
        if ( remove_success ) {
            std::string without_comments;
            remove_success = remove_multi_line_comments( without_one_line_comments
                                                         , without_comments );
            if ( remove_success ) {
                resplit( without_comments, obtained_strings );
                remove_empty_strings_from( obtained_strings );
            } else {
                notify( "Something wrong with multi-line comments!" );
            }
        } else {
            notify( "Something wrong with one-line comments!" );
        } 
    }
private:
    std::string concatenate( const str_storage& obtained_strings ) const {
        std::string result;
        BOOST_FOREACH ( const std::string& s, obtained_strings ) {
            result += s + '\n';
        }
        return result;
    }

    void resplit( const std::string& s, str_storage& obtained_strings ) const {
        obtained_strings.clear();
        boost::split( obtained_strings, s, boost::is_any_of( "\n" ) );
    }
private:
    bool remove_one_line_comments( const std::string& s, std::string& without_comments ) const {
        using boost::spirit::qi::_1;
        using boost::phoenix::push_back;
        using boost::spirit::qi::lit;
        using boost::spirit::qi::char_;
        using boost::spirit::qi::eol;
        using boost::spirit::qi::lexeme;
        
        simple_rule comments =    lit( one_line_comment_sign ) 
                               >> *( char_ - eol );
        simple_rule native = 
                lexeme[ *( char_[ push_back( boost::phoenix::ref(without_comments), _1 ) ] 
                           - lit( one_line_comment_sign ) ) ];
        simple_rule all = native % comments;
        return factual_parse( s, all );
    }

    bool remove_multi_line_comments( const std::string& s, std::string& without_comments ) const {
        using boost::spirit::qi::_1;
        using boost::phoenix::push_back;
        using boost::spirit::qi::lit;
        using boost::spirit::qi::char_;
        using boost::spirit::qi::lexeme;

        simple_rule comments =    lit( multi_line_comment_begin_sign ) 
                               >> *( char_ - lit( multi_line_comment_end_sign ) ) 
                               >> lit( multi_line_comment_end_sign );
        simple_rule native = 
                lexeme[ *( char_[ push_back( boost::phoenix::ref(without_comments), _1 ) ] 
                           - lit( multi_line_comment_begin_sign ) ) ];
        simple_rule all = native % comments;
        return factual_parse( s, all );
    }

    bool factual_parse( const std::string& s, const simple_rule& all ) const {
        string_const_it first = s.begin();
        string_const_it last  = s.end();

        bool parse_success = boost::spirit::qi::parse( first, last, all );
        return parse_success && first == last;
    }
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_COMMENTS_REMOVER_HPP
