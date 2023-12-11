#pragma once

#include <koinos/util/conversion.hpp>

#include <string>

namespace koinos::util {

template< typename T >
std::string to_hex( const T& );

template< typename T >
T from_hex( const std::string& );

template<>
std::string to_hex< std::string >( const std::string& s );

template<>
std::string from_hex< std::string >( const std::string& s );

template< typename T >
std::string to_hex( const T& t )
{
  return to_hex( converter::as< std::string >( t ) );
}

template< typename T >
T from_hex( const std::string& s )
{
  return converter::to< T >( from_hex< std::string >( s ) );
}

} // namespace koinos::util
