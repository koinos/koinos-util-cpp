#pragma once

#include <boost/endian/conversion.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace koinos {

using int128_t  = boost::multiprecision::int128_t;
using uint128_t = boost::multiprecision::uint128_t;
using int256_t  = boost::multiprecision::int256_t;
using uint256_t = boost::multiprecision::uint256_t;

using int160_t = boost::multiprecision::number<
   boost::multiprecision::cpp_int_backend<
      160,
      160,
      boost::multiprecision::signed_magnitude,
      boost::multiprecision::unchecked, void
   >
>;

using uint160_t = boost::multiprecision::number<
   boost::multiprecision::cpp_int_backend<
      160,
      160,
      boost::multiprecision::unsigned_magnitude,
      boost::multiprecision::unchecked, void
   >
>;

template< typename Stream >
inline void to_binary( Stream& s, const int128_t& v )
{
   auto hi = boost::endian::native_to_big( int64_t( v >> 64 ) );
   auto lo = boost::endian::native_to_big( uint64_t( v ) );
   s.write( (const char*)&hi, sizeof(hi) );
   s.write( (const char*)&lo, sizeof(lo) );
}

template< typename Stream >
inline void from_binary( Stream& s, int128_t& v )
{
   int64_t hi;
   uint64_t lo;
   s.read( (char*)&hi, sizeof(hi) );
   s.read( (char*)&lo, sizeof(lo) );

   v = boost::endian::big_to_native( hi );
   v <<= 64;
   v |= boost::endian::big_to_native( lo );
}

template< typename Stream >
inline void to_binary( Stream& s, const uint128_t& v )
{
   auto hi = boost::endian::native_to_big( uint64_t( v >> 64 ) );
   auto lo = boost::endian::native_to_big( uint64_t( v ) );
   s.write( (const char*)&hi, sizeof(hi) );
   s.write( (const char*)&lo, sizeof(lo) );
}

template< typename Stream >
inline void from_binary( Stream& s, uint128_t& v )
{
   uint64_t hi, lo;
   s.read( (char*)&hi, sizeof(hi) );
   s.read( (char*)&lo, sizeof(lo) );

   v = boost::endian::big_to_native( hi );
   v <<= 64;
   v |= boost::endian::big_to_native( lo );
}

template< typename Stream >
inline void to_binary( Stream& s, const int160_t& v )
{
   auto hi = boost::endian::native_to_big( int32_t( v >> 128 ) );
   auto lo = uint128_t( v );
   s.write( (const char*)&hi, sizeof(hi) );
   to_binary( s, lo );
}

template< typename Stream >
inline void from_binary( Stream& s, int160_t& v )
{
   int32_t hi;
   uint128_t lo;
   s.read( (char*)&hi, sizeof(hi) );
   from_binary( s, lo );

   v = boost::endian::big_to_native( hi );
   v <<= 128;
   v |= lo;
}

template< typename Stream >
inline void to_binary( Stream& s, const uint160_t& v )
{
   auto hi = boost::endian::native_to_big( uint32_t( v >> 128 ) );
   auto lo = uint128_t( v );
   s.write( (const char*)&hi, sizeof(hi) );
   to_binary( s, lo );
}

template< typename Stream >
inline void from_binary( Stream& s, uint160_t& v )
{
   uint32_t hi;
   uint128_t lo;
   s.read( (char*)&hi, sizeof(hi) );
   from_binary( s, lo );

   v = boost::endian::big_to_native( hi );
   v <<= 128;
   v |= lo;
}

template< typename Stream >
inline void to_binary( Stream& s, const int256_t& v )
{
   auto hi = int128_t( v >> 128 );
   auto lo = uint128_t( v );
   to_binary( s, hi );
   to_binary( s, lo );
}

template< typename Stream >
inline void from_binary( Stream& s, int256_t& v )
{
   int128_t hi;
   uint128_t lo;
   from_binary( s, hi );
   from_binary( s, lo );

   v = hi;
   v <<= 128;
   v |= lo;
}

template< typename Stream >
inline void to_binary( Stream& s, const uint256_t& v )
{
   auto hi = uint128_t( v >> 128 );
   auto lo = uint128_t( v );
   to_binary( s, hi );
   to_binary( s, lo );
}

template< typename Stream >
inline void from_binary( Stream& s, uint256_t& v )
{
   uint128_t hi, lo;
   from_binary( s, hi );
   from_binary( s, lo );

   v = hi;
   v <<= 128;
   v |= lo;
}

} // koinos
