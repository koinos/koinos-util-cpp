#pragma once

#include <sstream>

namespace google::protobuf {
   class Message;
}

namespace koinos::converter {

namespace detail {

template< typename T, std::size_t N >
void maybe_resize( std::array< T, N >&, std::size_t ) {}

template< class Container >
void maybe_resize( Container& c, std::size_t s )
{
   c.resize( s );
}

template< class Container, typename T >
typename std::enable_if_t< std::is_base_of_v< google::protobuf::Message, T >, std::size_t >
as_impl( Container& c, const T& t, std::size_t start )
{
   std::string s;
   t.SerializeToString( &s );

   detail::maybe_resize( c, c.size() + s.size() );

   std::transform( std::begin( s ), std::end( s ), std::begin( c ) + start, []( char ch ) { return reinterpret_cast< decltype( *std::begin( c ) ) >( ch ); } );

   return start + s.size();
}

template< class Container, typename T >
typename std::enable_if_t< !std::is_base_of_v< google::protobuf::Message, T >, std::size_t >
as_impl( Container& c, const T& t, std::size_t start )
{
   std::stringstream ss;
   to_binary( ss, t );
   auto s = ss.str();

   detail::maybe_resize( c, c.size() + s.size() );

   std::transform( std::begin( s ), std::end( s ), std::begin( c ) + start, []( char ch ) { return reinterpret_cast< decltype( *std::begin( c ) ) >( ch ); } );

   return start + s.size();
}

template< typename T >
typename std::enable_if_t< std::is_base_of_v< google::protobuf::Message, T >, void >
to_impl( std::stringstream& ss, T& t )
{
   t.ParseFromIstream( &ss );
}

template< typename T >
typename std::enable_if_t< !std::is_base_of_v< google::protobuf::Message, T >, void >
to_impl( std::stringstream& ss, T& t )
{
   from_binary( ss, t );
}

template< class Container >
void as_n( Container& c, std::size_t start ) {}

template< class Container, typename T, typename... Ts >
void as_n( Container& c, std::size_t start, const T& t, Ts... ts )
{
   static_assert( !std::is_base_of_v< google::protobuf::Message, T > );

   start += as_impl( c, t, start );
   as_n( c, start, ts... );
}

// Is there a std or boost type that does this?
template< int A, int B >
struct greater_than
{
   static constexpr bool value = A > B;
};

template< typename T >
std::tuple< T > to_n( std::stringstream& ss )
{
   static_assert( !std::is_base_of_v< google::protobuf::Message, T > );

   T t;
   to_impl( ss, t );

   return std::tuple< T >( std::move( t ) );
}

template< typename T, typename... Ts >
std::enable_if_t< greater_than< sizeof...( Ts ), 0 >::value, std::tuple< T, Ts... > >
to_n( std::stringstream& ss )
{
   static_assert( !std::is_base_of_v< google::protobuf::Message, T > );

   T t;
   to_impl( ss, t );

   return std::tuple_cat( std::tuple< T >( std::move( t ) ), to_n< Ts... >( ss ) );
}

} // detail

template< class Container, typename... Ts >
Container as( Ts... ts )
{
   Container c;
   static_assert( sizeof( *c.begin() ) == sizeof( std::byte ) );
   detail::as_n( c, 0, ts... );

   return c;
}

template< typename T, class Container >
T to( Container& c )
{
   static_assert( sizeof( *std::begin( c ) ) == sizeof( std::byte ) );
   std::stringstream ss;

   for ( const auto& e : c )
      ss.write( reinterpret_cast< const char * >( &e ), sizeof( std::byte ) );

   T t;
   detail::to_impl< T >( ss, t );

   return t;
}

template< typename... Ts, class Container >
std::enable_if_t< detail::greater_than< sizeof...( Ts ), 1 >::value, std::tuple< Ts... > >
to( const Container& c )
{
   static_assert( sizeof( *std::begin( c ) ) == sizeof( std::byte ) );
   std::stringstream ss;

   for ( const auto& e : c )
      ss.write( reinterpret_cast< const char * >( &e ), sizeof( std::byte ) );

   return detail::to_n<  Ts... >( ss );
}

} // koinos
