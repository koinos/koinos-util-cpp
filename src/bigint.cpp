#include <koinos/bigint.hpp>

namespace koinos {

template<>
void to_binary< int128_t >( std::ostream& s, const int128_t& v )
{
   auto hi = boost::endian::native_to_big( int64_t( v >> 64 ) );
   auto lo = boost::endian::native_to_big( uint64_t( v ) );
   s.write( (const char*)&hi, sizeof(hi) );
   s.write( (const char*)&lo, sizeof(lo) );
}

template<>
void from_binary< int128_t >( std::istream& s, int128_t& v )
{
   int64_t hi;
   uint64_t lo;
   s.read( (char*)&hi, sizeof(hi) );
   s.read( (char*)&lo, sizeof(lo) );

   v = boost::endian::big_to_native( hi );
   v <<= 64;
   v |= boost::endian::big_to_native( lo );
}

template<>
void to_binary< uint128_t >( std::ostream& s, const uint128_t& v )
{
   auto hi = boost::endian::native_to_big( uint64_t( v >> 64 ) );
   auto lo = boost::endian::native_to_big( uint64_t( v ) );
   s.write( (const char*)&hi, sizeof(hi) );
   s.write( (const char*)&lo, sizeof(lo) );
}

template<>
void from_binary< uint128_t >( std::istream& s, uint128_t& v )
{
   uint64_t hi, lo;
   s.read( (char*)&hi, sizeof(hi) );
   s.read( (char*)&lo, sizeof(lo) );

   v = boost::endian::big_to_native( hi );
   v <<= 64;
   v |= boost::endian::big_to_native( lo );
}

template<>
void to_binary< int160_t >( std::ostream& s, const int160_t& v )
{
   auto hi = boost::endian::native_to_big( int32_t( v >> 128 ) );
   auto lo = uint128_t( v );
   s.write( (const char*)&hi, sizeof(hi) );
   to_binary( s, lo );
}

template<>
void from_binary< int160_t >( std::istream& s, int160_t& v )
{
   int32_t hi;
   uint128_t lo;
   s.read( (char*)&hi, sizeof(hi) );
   from_binary( s, lo );

   v = boost::endian::big_to_native( hi );
   v <<= 128;
   v |= lo;
}

template<>
void to_binary< uint160_t >( std::ostream& s, const uint160_t& v )
{
   auto hi = boost::endian::native_to_big( uint32_t( v >> 128 ) );
   auto lo = uint128_t( v );
   s.write( (const char*)&hi, sizeof(hi) );
   to_binary( s, lo );
}

template<>
void from_binary< uint160_t >( std::istream& s, uint160_t& v )
{
   uint32_t hi;
   uint128_t lo;
   s.read( (char*)&hi, sizeof(hi) );
   from_binary( s, lo );

   v = boost::endian::big_to_native( hi );
   v <<= 128;
   v |= lo;
}

template<>
void to_binary< int256_t >( std::ostream& s, const int256_t& v )
{
   auto hi = int128_t( v >> 128 );
   auto lo = uint128_t( v );
   to_binary( s, hi );
   to_binary( s, lo );
}

template<>
void from_binary< int256_t >( std::istream& s, int256_t& v )
{
   int128_t hi;
   uint128_t lo;
   from_binary( s, hi );
   from_binary( s, lo );

   v = hi;
   v <<= 128;
   v |= lo;
}

template<>
void to_binary< uint256_t >( std::ostream& s, const uint256_t& v )
{
   auto hi = uint128_t( v >> 128 );
   auto lo = uint128_t( v );
   to_binary( s, hi );
   to_binary( s, lo );
}

template<>
void from_binary< uint256_t >( std::istream& s, uint256_t& v )
{
   uint128_t hi, lo;
   from_binary( s, hi );
   from_binary( s, lo );

   v = hi;
   v <<= 128;
   v |= lo;
}

} // koinos
