#include <sstream>

namespace google::protobuf {
   class Message;
}

namespace koinos::converter {

template< class Container, typename T >
typename std::enable_if_t< std::is_base_of_v< google::protobuf::Message, T >, Container >
as( const T& t )
{
   std::string s;
   t.SerializeToString( &s );

   Container b;
   b.resize( s.size() );

   std::transform( s.begin(), s.end(), b.begin(), []( char c ) { return reinterpret_cast< decltype( *b.begin() ) >( c ); } );

   static_assert( sizeof( *b.begin() ) == sizeof( std::byte ) );

   return b;
}

template< class Container, typename T >
typename std::enable_if_t< !std::is_base_of_v< google::protobuf::Message, T >, Container >
as( const T& t )
{
   std::stringstream ss;
   to_binary( ss, t );
   auto s = ss.str();

   Container b;
   b.resize( s.size() );

   std::transform( s.begin(), s.end(), b.begin(), []( char c ) { return reinterpret_cast< decltype( *b.begin() ) >( c ); } );

   static_assert( sizeof( *b.begin() ) == sizeof( std::byte ) );

   return b;
}

template< typename T, class Container >
typename std::enable_if_t< std::is_base_of_v< google::protobuf::Message, T >, T >
from( const Container& c )
{
   T t;
   std::stringstream ss;

   static_assert( sizeof( *c.begin() ) == sizeof( std::byte ) );

   for ( const auto& e : c )
      ss.write( reinterpret_cast< const char * >( &e ), sizeof( std::byte ) );

   t.ParseFromIstream( &ss );

   return t;
}

template< typename T, class Container >
typename std::enable_if_t< !std::is_base_of_v< google::protobuf::Message, T >, T >
from( const Container& c )
{
   T t;
   std::stringstream ss;

   static_assert( sizeof( *c.begin() ) == sizeof( std::byte ) );

   for ( const auto& e : c )
      ss.write( reinterpret_cast< const char * >( &e ), sizeof( std::byte ) );

   from_binary( ss, t );

   return t;
}

} // koinos
