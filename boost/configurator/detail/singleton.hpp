// detail/singleton.hpp
// ~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_SINGLETON_HPP
#define BOOST_CONFIGURATOR_SINGLETON_HPP

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#include <cstdlib>

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \class singleton
/// \brief Realization of 'Singleton' pattern.
template< typename T >
class singleton : boost::noncopyable {
public:
    static boost::mutex mutex;
private:
    static T* t;
public: 
    static T& inst() {
        if ( 0 == t ) {
            boost::mutex::scoped_lock lock( mutex );
            if ( 0 == t ) {  
                t = new T();
                ::atexit( destroy );
            } else {}
        } else {}
        return *t;
    }
private:
    static void destroy() {
        boost::mutex::scoped_lock lock( mutex );
        delete t;
        t = 0;
    }
};

template< typename T >
boost::mutex singleton< T >::mutex;

template< typename T >
T* singleton< T >::t = 0;

} // namespace detail
} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_SINGLETON_HPP
