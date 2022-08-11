#pragma once

#include <koinos/util/conversion.hpp>

#include <string>

namespace koinos::util {

template< typename T >
std::string to_base64( const T&, bool websafe = true );

template< typename T >
T from_base64( const std::string& );

template<>
std::string to_base64< std::string >( const std::string&, bool websafe );

template<>
std::string from_base64< std::string >( const std::string& s );

template< typename T >
std::string to_base64( const T& t, bool websafe )
{
   return to_base64( converter::as< std::string >( t ), websafe );
}

template< typename T >
T from_base64( const std::string& s )
{
   return converter::to< T >( from_base64< std::string >( s ) );
}

} // koinos::util
