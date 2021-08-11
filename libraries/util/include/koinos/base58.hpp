#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include <koinos/util.hpp>

// Base58 algorithm is from Bitcoin's implementation
// Copyright (c) 2014-2019 The Bitcoin Core developers
// Distributed under the MIT software license
// http://www.opensource.org/licenses/mit-license.php.

namespace koinos {

namespace detail::base58 {
   constexpr std::size_t max_array_size = 1024 * 1024 * 10;
} // detail::base58

bool decode_base58( const char* psz, std::vector< char >& dest, std::size_t max_ret_len = detail::base58::max_array_size );
void decode_base58( const char* begin, std::size_t count, std::vector< char >& dest );

void decode_base58( const std::string& src, std::vector< char >& dest );
void decode_base58( const std::string& src, std::vector< unsigned char >& dest );
void decode_base58( const std::string& src, std::vector< std::byte >& dest );

template< size_t N >
void decode_base58( const std::string& src, std::array< char, N >& dest )
{
   static_assert( N < detail::base58::max_array_size );

   std::vector< char > v;

   KOINOS_TODO( "This is inefficient. Consider ways to optimize base58 for an array" );
   if( !decode_base58( src.c_str(), v, N ) || v.size() != N )
      throw std::runtime_error( "failed during base58 decode" );

   std::memcpy( dest.data(), v.data(), N );
}

template< size_t N >
void decode_base58( const std::string& src, std::array< unsigned char, N >& dest )
{
   static_assert( N < detail::base58::max_array_size );

   std::vector< char > v;

   KOINOS_TODO( "This is inefficient. Consider ways to optimize base58 for an array" );
   if( !decode_base58( src.c_str(), v, N ) || v.size() != N )
      throw std::runtime_error( "failed during base58 decode" );

   std::memcpy( dest.data(), v.data(), N );
}

template< size_t N >
void decode_base58( const std::string& src, std::array< std::byte, N >& dest )
{
   static_assert( N < detail::base58::max_array_size );

   std::vector< char > v;

   KOINOS_TODO( "This is inefficient. Consider ways to optimize base58 for an array" );
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
   const unsigned char* begin = static_cast< const unsigned char* >( v.data() );
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
   const unsigned char* begin = static_cast< const unsigned char* >( v.data() );
   return encode_base58( begin, begin + N );
}

} // koinos
