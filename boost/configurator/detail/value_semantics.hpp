// detail/value_semantics.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// Copyright (C) 2010 Denis Shevchenko (for @ dshevchenko.biz)
//
// Distributed under the Boost Software License, version 1.0
// (see http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONFIGURATOR_VALUE_SEMANTICS_HPP
#define BOOST_CONFIGURATOR_VALUE_SEMANTICS_HPP

namespace boost {

/// \namespace cf
/// \brief Main namespace of library.
namespace cf {

/// \namespace cf::detail
/// \brief Details of realization.
namespace detail {

/// \enum value_semantic 
/// \brief Value semantic types (for check).
enum value_semantic {
    no_semantic			/*!< Default value. */
	, path				/*!< Path semantics check. */
	, optional_path	    /*!< Optional path semantics check. */
	, ipv4				/*!< IPv4 semantics check. */
    , ipv6              /*!< IPv6 semantics check. */
    , ip                /*!< IP semantics check. */
    , email             /*!< E-mail semantics check. */
    , size              /*!< Size semantics check. */
    , time_period       /*!< Time period semantics check. */
    , exp_record        /*!< Exponential record semantics check. */
};

///
struct common_semantic {
    explicit common_semantic( const detail::value_semantic& _semantic ) :
            semantic( _semantic ) {}
public:
    const detail::value_semantic semantic;
};

#define BOOST_CONFIGURATOR_SEMANTIC( type )                   \
    struct type : public detail::common_semantic {            \
        type() : detail::common_semantic( detail::type ) {}   \
    };

} // namespace detail

BOOST_CONFIGURATOR_SEMANTIC( no_semantic )
BOOST_CONFIGURATOR_SEMANTIC( path )
BOOST_CONFIGURATOR_SEMANTIC( optional_path )
BOOST_CONFIGURATOR_SEMANTIC( ipv4 )
BOOST_CONFIGURATOR_SEMANTIC( ipv6 )
BOOST_CONFIGURATOR_SEMANTIC( ip )
BOOST_CONFIGURATOR_SEMANTIC( email )
BOOST_CONFIGURATOR_SEMANTIC( size )
BOOST_CONFIGURATOR_SEMANTIC( time_period )
BOOST_CONFIGURATOR_SEMANTIC( exp_record )

} // namespace cf
} // namespace boost

#endif // BOOST_CONFIGURATOR_VALUE_SEMANTICS_HPP
