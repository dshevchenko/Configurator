// Example of simplest usage of Configurator library.
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#include <boost/configurator/configurator.hpp>

#include <iostream>

BOOST_CONFIGURATOR_OPTION( Host )

int main( int argc, char* argv[] ) {	
    try {
        boost::cf::configurator conf; 
        conf.add< Host>();

        conf.load( "/some/path/to/hello_world_conf.txt" ); // See hello_world_conf.txt in 'example'.

        std::string host = conf.get_value_of< Host >();
        std::cout << "Host: " << host << std::endl;
    } catch ( const std::exception& exc ) {
		std::cerr << exc.what() << std::endl;
	} 
	
	return 0;
}
