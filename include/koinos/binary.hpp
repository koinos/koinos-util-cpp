#pragma once

#include <iostream>
#include <type_traits>

#include <boost/endian/conversion.hpp>

namespace koinos {

template< typename T = void >
inline void to_binary( std::ostream&, const T& ) = delete;

template< typename T = void >
inline void from_binary( std::istream&, T& ) = delete;

#define KOINOS_DEFINE_INT_SERIALIZER( int_type )                        \
template<>                                                              \
inline void to_binary< int_type >( std::ostream& s, const int_type& t ) \
{                                                                       \
   int_type b = boost::endian::native_to_big( t );                      \
   s.write( reinterpret_cast< const char* >( &b ), sizeof( b ) );       \
}                                                                       \
template<>                                                              \
inline void from_binary< int_type >( std::istream& s, int_type& t )     \
{                                                                       \
   s.read( reinterpret_cast< char * >( &t ), sizeof( t )  );            \
   if ( !s.good() ) throw std::logic_error( "Error reading from stream" ); \
   boost::endian::big_to_native_inplace( t );                           \
}

KOINOS_DEFINE_INT_SERIALIZER( int8_t )
KOINOS_DEFINE_INT_SERIALIZER( uint8_t )
KOINOS_DEFINE_INT_SERIALIZER( int16_t )
KOINOS_DEFINE_INT_SERIALIZER( uint16_t )
KOINOS_DEFINE_INT_SERIALIZER( int32_t )
KOINOS_DEFINE_INT_SERIALIZER( uint32_t )
KOINOS_DEFINE_INT_SERIALIZER( int64_t )
KOINOS_DEFINE_INT_SERIALIZER( uint64_t )

#undef KOINOS_DEFINE_INT_SERIALIZER

} // koinos
