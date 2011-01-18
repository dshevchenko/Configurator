// detail/pure_strings_obtainer.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_PURE_STRINGS_OBTAINER_HPP
#define BOOST_CONFIGURATOR_PURE_STRINGS_OBTAINER_HPP

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include <string>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

namespace fs = boost::filesystem;

/// \class pure_strings_obtainer
/// \brief Configuration's file strings obtainer.
///
/// Obtains pure strings from configuration file.
class pure_strings_obtainer {
private:
    fs::path path_to_configuration_file;
public:
    str_storage operator()( const std::string& _path_to_configuration_file ) {
        path_to_configuration_file = _path_to_configuration_file;
        return obtained_strings();
    }

    std::string stored_path() const {
        return path_to_configuration_file.string();
    } 
private:
    str_storage obtained_strings() const {
        fs::ifstream configuration_file;
        open_file( configuration_file );
        return obtained_strings_from( configuration_file );
    }

    void open_file( fs::ifstream& configuration_file ) const {
        configuration_file.open( path_to_configuration_file );
        if ( !configuration_file.good() ) {
            notify( "Cannot open configuration file '" + path_to_configuration_file.string()
                    + "', probably you have not permissions..." );
        } else {}
    }
    
    str_storage obtained_strings_from( fs::ifstream& configuration_file ) const {
        str_storage strings;
        std::string s;
        while ( std::getline( configuration_file, s ) ) {
            boost::trim( s );
            strings += s;
        }
        return strings;
    } 
};

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_PURE_STRINGS_OBTAINER_HPP
