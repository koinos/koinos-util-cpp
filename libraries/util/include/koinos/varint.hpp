#pragma once

#include <cstdint>
#include <stdexcept>

namespace koinos {

struct signed_varint
{
   signed_varint( int64_t v ) : value( v ) {}
   signed_varint() = default;
   int64_t value = 0;
};

struct unsigned_varint
{
   unsigned_varint( uint64_t v ) : value( v ) {}
   unsigned_varint() = default;
   uint64_t value = 0;
};

template< typename Stream >
inline void to_binary( Stream& s, const unsigned_varint& v )
{
   char tmp[ ( sizeof( v.value ) * 8 + 6 ) / 7 ];
   uint64_t n = v.value;
   uint32_t len = 0;

   while ( true )
   {
      tmp[len] = ( n & 0x7F );

      if ( n <= 0x7F )
         break;

      tmp[len] |= 0x80;
      n = ( n >> 7 );
      len++;
   }

   for ( uint32_t i = 0; i <= len; i++ )
   {
      s.write( tmp + i, sizeof( char ) );
   }
}

template< typename Stream >
inline void from_binary( Stream& s, unsigned_varint& v )
{
   v.value = 0;
   char data = 0;
   uint32_t i = 0;
   do
   {
      s.get( data );

      if ( !( s.good() ) )
         throw std::runtime_error( "error reading from stream" );

      v.value |= ( data & 0x7f ) << i;
      i += 7;
   } while( data & 0x80 );
}

template< typename Stream >
inline void to_binary( Stream& s, const signed_varint& v )
{
   unsigned_varint val = 0;
   val.value = uint64_t( ( v.value << 1 ) ^ ( v.value >> 63 ) );
   to_binary( s, val );
}

template< typename Stream >
inline void from_binary( Stream& s, signed_varint& v )
{
   unsigned_varint val;
   from_binary( s, val );

   bool neg = val.value & 0x01;

   v.value = ( ( val.value >> 1 ) ^ ( val.value >> 63 ) ) + ( val.value & 0x01 );
   if ( neg )
      v.value = -v.value;
}

} // koinos
