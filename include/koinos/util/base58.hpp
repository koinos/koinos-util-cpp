#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include <koinos/util/conversion.hpp>

// Base58 algorithm is from Bitcoin's implementation
// Copyright (c) 2014-2019 The Bitcoin Core developers
// Distributed under the MIT software license
// http://www.opensource.org/licenses/mit-license.php.

namespace koinos::util {

namespace detail::base58 {
constexpr std::size_t max_array_size = 1'024 * 1'024 * 10;
} // namespace detail::base58

bool decode_base58( const char* psz,
                    std::vector< char >& dest,
                    std::size_t max_ret_len = detail::base58::max_array_size );
void decode_base58( const char* begin, std::size_t count, std::vector< char >& dest );

void decode_base58( const std::string& src, std::vector< char >& dest );
void decode_base58( const std::string& src, std::vector< unsigned char >& dest );
void decode_base58( const std::string& src, std::vector< std::byte >& dest );

template< size_t N >
void decode_base58( const std::string& src, std::array< char, N >& dest )
{
  static_assert( N < detail::base58::max_array_size );

  std::vector< char > v;

  if( !decode_base58( src.c_str(), v, N ) || v.size() != N )
    throw std::runtime_error( "failed during base58 decode" );

  std::memcpy( dest.data(), v.data(), N );
}

template< size_t N >
void decode_base58( const std::string& src, std::array< unsigned char, N >& dest )
{
  static_assert( N < detail::base58::max_array_size );

  std::vector< char > v;

  if( !decode_base58( src.c_str(), v, N ) || v.size() != N )
    throw std::runtime_error( "failed during base58 decode" );

  std::memcpy( dest.data(), v.data(), N );
}

template< size_t N >
void decode_base58( const std::string& src, std::array< std::byte, N >& dest )
{
  static_assert( N < detail::base58::max_array_size );

  std::vector< char > v;

  if( !decode_base58( src.c_str(), v, N ) || v.size() != N )
    throw std::runtime_error( "failed during base58 decode" );

  std::memcpy( dest.data(), v.data(), N );
}

std::string encode_base58( const unsigned char* begin, const unsigned char* end );
std::string encode_base58( const std::vector< char >& v );
std::string encode_base58( const std::vector< unsigned char >& v );
std::string encode_base58( const std::vector< std::byte >& v );

template< size_t N >
std::string encode_base58( const std::array< char, N >& v )
{
  const unsigned char* begin = reinterpret_cast< const unsigned char* >( v.data() );
  return encode_base58( begin, begin + N );
}

template< size_t N >
std::string encode_base58( const std::array< unsigned char, N >& v )
{
  const unsigned char* begin = v.data();
  return encode_base58( begin, begin + N );
}

template< size_t N >
std::string encode_base58( const std::array< std::byte, N >& v )
{
  const unsigned char* begin = reinterpret_cast< const unsigned char* >( v.data() );
  return encode_base58( begin, begin + N );
}

template< typename T >
std::string to_base58( const T& );
template< typename T >
T from_base58( const std::string& );

template<>
std::string to_base58< std::string >( const std::string& );
template<>
std::string from_base58< std::string >( const std::string& );

template<>
std::string to_base58< std::vector< char > >( const std::vector< char >& );
template<>
std::vector< char > from_base58< std::vector< char > >( const std::string& );

template<>
std::string to_base58< std::vector< unsigned char > >( const std::vector< unsigned char >& );
template<>
std::vector< unsigned char > from_base58< std::vector< unsigned char > >( const std::string& );

template<>
std::string to_base58< std::vector< std::byte > >( const std::vector< std::byte >& );
template<>
std::vector< std::byte > from_base58< std::vector< std::byte > >( const std::string& );

template< typename T >
std::string to_base58( const T& t )
{
  return encode_base58( converter::as< std::vector< std::byte > >( t ) );
}

template< typename T >
T from_base58( const std::string& s )
{
  std::vector< std::byte > v;
  decode_base58( s, v );
  return converter::to< T >( v );
}

} // namespace koinos::util
