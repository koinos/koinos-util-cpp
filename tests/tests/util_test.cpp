#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <koinos/tests/util_fixture.hpp>
#include <koinos/util/base58.hpp>
#include <koinos/util/conversion.hpp>
#include <koinos/util/hex.hpp>
#include <koinos/util/options.hpp>
#include <koinos/binary.hpp>
#include <koinos/varint.hpp>

#include <boost/program_options.hpp>

BOOST_FIXTURE_TEST_SUITE( util_tests, util_fixture )

BOOST_AUTO_TEST_CASE( varint_test )
{
   BOOST_TEST_MESSAGE( "signed_varint" );

   int64_t a  = -10;

   std::stringstream stream_a;

   koinos::to_binary( stream_a, koinos::signed_varint( a ) );

   koinos::signed_varint v_a;

   koinos::from_binary( stream_a, v_a );

   BOOST_CHECK_EQUAL( v_a.value, a );

   BOOST_TEST_MESSAGE( "unsigned_varint" );

   uint64_t b = 10;

   std::stringstream stream_b;

   koinos::to_binary( stream_b, koinos::unsigned_varint( b ) );

   koinos::unsigned_varint v_b;

   koinos::from_binary( stream_b, v_b );

   BOOST_CHECK_EQUAL( v_b.value, b );
}

BOOST_AUTO_TEST_CASE( base58_test )
{
   constexpr std::size_t array_size = 43;
   constexpr std::size_t array_size_overflow = 42;
   std::string str     = "The quick brown fox jumps over the lazy dog";
   std::string b58_str = "7DdiPPYtxLjCD3wA1po2rvZHTDYjkZYiEtazrfiwJcwnKCizhGFhBGHeRdx";

   BOOST_TEST_MESSAGE( "base58 std::vector< char >" );
   {
      std::vector< char > bs;
      koinos::util::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += c;

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< unsigned char >" );
   {
      std::vector< unsigned char > bs;
      koinos::util::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += static_cast< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< std::byte >" );
   {
      std::vector< std::byte > bs;
      koinos::util::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += std::to_integer< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< char, N >" );
   {
      std::array< char, array_size > bs;
      koinos::util::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += c;

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< unsigned char, N >" );
   {
      std::array< unsigned char, array_size > bs;
      koinos::util::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += static_cast< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< std::byte, N >" );
   {
      std::array< std::byte, array_size > bs;
      koinos::util::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += std::to_integer< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< char, N > overflow" );
   {
      std::array< char, array_size_overflow > bs;
      BOOST_CHECK_THROW( koinos::util::decode_base58( b58_str, bs ), std::runtime_error );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< unsigned char, N > overflow" );
   {
      std::array< unsigned char, array_size_overflow > bs;
      BOOST_CHECK_THROW( koinos::util::decode_base58( b58_str, bs ), std::runtime_error );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< std::byte, N > overflow" );
   {
      std::array< std::byte, array_size_overflow > bs;
      BOOST_CHECK_THROW( koinos::util::decode_base58( b58_str, bs ), std::runtime_error );
   }
}

BOOST_AUTO_TEST_CASE( conversion_test )
{
   uint32_t x = 10;

   auto s = koinos::util::converter::as< std::string >( x );
   auto y = koinos::util::converter::to< uint32_t >( s );

   BOOST_REQUIRE( x == y );

   y = 20;
   uint64_t z = 30;

   s = koinos::util::converter::as< std::string >( x, z );

   auto [a, b] = koinos::util::converter::to< uint32_t, uint64_t >( s );

   BOOST_REQUIRE( x == a );
   BOOST_REQUIRE( z == b );

   auto t = koinos::util::converter::as< std::string >( z );

   s = koinos::util::converter::as< std::string >( x, t );

   auto [c, d] = koinos::util::converter::to< uint64_t, uint32_t >( s );

   BOOST_REQUIRE( c == uint64_t( x ) << 32 );
   BOOST_REQUIRE( d == uint32_t( z ) );

   auto v = koinos::util::converter::as< std::vector< std::byte > >( s );

   auto [e, u] = koinos::util::converter::to< uint32_t, std::string >( v );

   BOOST_REQUIRE( e == x );
   BOOST_REQUIRE( t == u );

   auto [f, g ] = koinos::util::converter::to< uint32_t, uint16_t >( std::string( "foobar" ) );

   BOOST_REQUIRE( f == 0x666f6f62 );
   BOOST_REQUIRE( g == 0x6172 );

   auto w = koinos::util::converter::as< std::vector< std::byte > >( std::string( "foobar" ) );
   s = koinos::util::converter::as< std::string >( w );

   BOOST_REQUIRE( s == "foobar" );
}

BOOST_AUTO_TEST_CASE( hex_test )
{
   const char bytes[] = {4, 8, 15, 16, 23, 42};
   std::string byte_str( bytes, sizeof( bytes ) );

   auto hex_str = koinos::util::to_hex( byte_str );

   BOOST_CHECK( hex_str == "0x04080f10172a" );

   auto new_byte_str = koinos::util::from_hex( hex_str );

   BOOST_CHECK( byte_str == new_byte_str );
}

BOOST_AUTO_TEST_CASE( options_test )
{
   boost::program_options::variables_map cli_args;
   YAML::Node service_config;
   YAML::Node global_config;

   uint32_t default_value = 0;

   auto i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == default_value );

   global_config[ "foo" ] = 1;
   i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == global_config[ "foo" ].as< uint32_t >() );

   service_config[ "foo" ] = 2;
   i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == service_config[ "foo" ].as< uint32_t >() );

   char* args[] = {"test", "--foo", "2"};
   boost::program_options::options_description options;
   options.add_options()
      ("foo", boost::program_options::value< uint32_t >(), "test option" );

   boost::program_options::store( boost::program_options::parse_command_line( 3, args, options ), cli_args );
   i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == cli_args[ "foo" ].as< uint32_t >() );
}

BOOST_AUTO_TEST_SUITE_END()
