// option.hpp
// ~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_OPTION_HPP
#define BOOST_CONFIGURATOR_OPTION_HPP

#include <boost/configurator/detail/misc.hpp>
#include <boost/configurator/detail/value_semantics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/lexical_cast.hpp>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

///
struct option {
    explicit option( const std::string& _type_id, const std::string& _type_name ) :
            type_id( _type_id.begin(), _type_id.end() )
            , type_name( _type_name.begin(), _type_name.end() )
            , location( _type_name.begin(), _type_name.end() )
            , value()
            , semantic( detail::no_semantic )
            , necessary( false )
            , multi_values_allowed( false ) {}
    virtual ~option() {}
private:
    const std::string   type_id;
    const std::string   type_name;
public:
    std::string         location;
    std::string         section;
    std::string         value;
    detail::value_semantic semantic;
    bool                necessary;
    bool                multi_values_allowed;
    std::string         default_value;
public:
    option& set_location( const std::string& _location ) {
        location.assign( _location.begin(), _location.end() );
        detail::string_it end_it = boost::find_last( location, "::" ).begin();
        section.assign( location.begin(), end_it );
        return *this;
    }
public:    
    bool operator==( const std::type_info& type ) const {
        return type_id == type.name();
    }

    bool operator==( const std::string& _location ) const {
        return _location == location;
    }
 
    bool corresponds_by_section( const std::string& _section ) const {
        return _section == section;
    }
public: 
    template< typename Value >
    option& set_default_value( const Value& _value ) {
        check_option_necessity();
        value = boost::lexical_cast< std::string >( _value );
        default_value.assign( value.begin(), value.end() );
        return *this;
    }

    option& set_default_value( const std::string& _value ) {
        check_option_necessity();
        value.assign( _value.begin(), _value.end() );
        default_value.assign( value.begin(), value.end() );
        return *this;
    }

    option& set_default_value( const char* _value ) {
        check_option_necessity();
        value.assign( _value );
        default_value.assign( value.begin(), value.end() );
        return *this;
    }
private:
    void check_option_necessity() const {
        if ( necessary ) {
            detail::notify( "Option '" + type_name
                            + "' registered as necessary, so it cannot have default_value!" );
        } else {}
    }
public:
    option& set_necessity() {
        check_default_value_existence();
        necessary = true;
        return *this;
    }
private:
    void check_default_value_existence() const {
        if ( !default_value.empty() ) {
            detail::notify( "Option '" + type_name
                            + "' already have default value '" + default_value + "', "
                            + "so it cannot be necessary!" );
        } else {}
    }
public:
    template< typename Semantic >
    option& check_semantic() {
        Semantic sem;
        semantic = sem.semantic;
        return *this;
    }
public:
    option& allow_multi_values() {
        multi_values_allowed = true;
        return *this;
    }
public:
    bool semantic_defined() const           { return detail::no_semantic != semantic; }
    bool already_has_default_value() const  { return !default_value.empty(); }
    bool is_necessary() const               { return necessary; }
    bool empty() const                      { return value.empty(); }
};

typedef boost::ptr_vector< option > options;
typedef options::iterator           registered_option_it;
typedef options::const_iterator     registered_option_const_it;
typedef registered_option_it        option_it;
typedef registered_option_const_it  option_const_it;

} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_OPTION_HPP
