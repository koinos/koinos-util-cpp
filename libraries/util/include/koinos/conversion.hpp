#pragma once

#include <sstream>

#include <boost/mpl/greater.hpp>

#include <koinos/binary.hpp>

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

   maybe_resize( c, std::size( c ) + std::size( s ) );

   std::transform( std::begin( s ), std::end( s ), std::begin( c ) + start, []( char ch ) { return reinterpret_cast< decltype( *std::begin( c ) ) >( ch ); } );

   return start + std::size( s );
}

template< typename T, class = std::void_t<> >
struct is_container : std::false_type{};

template< typename T >
struct is_container< T, std::void_t<
   decltype(
      std::begin( std::declval< T >() ),
      std::end( std::declval< T >() ),
      std::size( std::declval< T >() ) ) > > : std::true_type{};

template< class Container, typename T >
typename std::enable_if_t< is_container< T >::value && !std::is_base_of_v< google::protobuf::Message, T >, std::size_t >
as_impl( Container& c, const T& t, std::size_t start )
{
   static_assert( sizeof( *std::begin( t ) ) == sizeof( std::byte ) );
   // If c is a dynamic container, std::size( c ) == start
   // If c is a static container (i.e. std::array, std::size( c ) != start )
   maybe_resize( c, std::size( c ) + std::size( t ) );

   std::size_t items_to_add = std::size( t );

   if ( std::size( c ) < start + std::size( t ) )
      items_to_add = std::size( c ) - start;

   // If c is a static container, we do not want to overflow, so only add up to the size of c
   std::transform( std::begin( t ), std::end( t ) - ( std::size( t ) - items_to_add ), std::begin( c ) + start, []( auto ch ) { return reinterpret_cast< decltype( *std::begin( c ) ) >( ch ); } );

   return start + items_to_add;
}

template< class Container, typename T >
typename std::enable_if_t< !is_container< T >::value && !std::is_base_of_v< google::protobuf::Message, T >, std::size_t >
as_impl( Container& c, const T& t, std::size_t start )
{
   std::stringstream ss;
   to_binary( ss, t );
   auto s = ss.str();

   maybe_resize( c, std::size( c ) + std::size( s ) );

   std::transform( std::begin( s ), std::end( s ), std::begin( c ) + start, []( char ch ) { return reinterpret_cast< decltype( *std::begin( c ) ) >( ch ); } );

   return start + std::size( s );
}

template< typename T >
typename std::enable_if_t< std::is_base_of_v< google::protobuf::Message, T >, void >
to_impl( std::stringstream& ss, T& t )
{
   t.ParseFromIstream( &ss );
}

template< typename T >
typename std::enable_if_t< is_container< T >::value && !std::is_base_of_v< google::protobuf::Message, T >, void >
to_impl( std::stringstream& ss, T& t )
{
   auto pos = ss.tellg();
   auto s = ss.str();

   maybe_resize( t, std::size( s ) - pos );

   std::transform( std::begin( s ) + pos, std::end( s ), std::begin( t ), []( auto ch ) { return reinterpret_cast< decltype( *std::begin( t ) ) >( ch ); } );
}

template< typename T >
typename std::enable_if_t< !is_container< T >::value && !std::is_base_of_v< google::protobuf::Message, T >, void >
to_impl( std::stringstream& ss, T& t )
{
   from_binary( ss, t );
}

// Is there a std or boost type that does this?
template< int A, int B >
struct greater_than
{
   static constexpr bool value = A > B;
};

template< class Container, typename T >
void as_n( Container& c, std::size_t start, T&& t )
{
   as_impl( c, t, start );
}

template< class Container, typename T, typename... Ts >
std::enable_if_t< greater_than< sizeof...( Ts ), 0 >::value, void >
as_n( Container& c, std::size_t start, const T& t, Ts... ts )
{
   static_assert( !std::is_base_of_v< google::protobuf::Message, T > );

   start += as_impl( c, t, start );
   as_n( c, start, std::forward< Ts >( ts )... );
}

template< typename T >
std::tuple< T > to_n( std::stringstream& ss )
{
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

template< class Container, typename T >
Container as( const T& t )
{
   Container c;
   static_assert( sizeof( *c.begin() ) == sizeof( std::byte ) );
   detail::as_impl( c, t, 0 );

   return c;
}

template< class Container, typename... Ts >
std::enable_if_t< detail::greater_than< sizeof...( Ts ), 1 >::value, Container >
as( Ts... ts )
{
   Container c;
   static_assert( sizeof( *c.begin() ) == sizeof( std::byte ) );
   detail::as_n( c, 0, std::forward< Ts >( ts )... );

   return c;
}

template< typename T, class Container >
T to( const Container& c )
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
