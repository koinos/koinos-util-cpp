#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <koinos/util/base58.hpp>
#include <koinos/util/base64.hpp>
#include <koinos/util/conversion.hpp>
#include <koinos/util/hex.hpp>
#include <koinos/util/options.hpp>
#include <koinos/binary.hpp>
#include <koinos/varint.hpp>

#include <boost/program_options.hpp>

struct util_fixture{};

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

   BOOST_TEST_MESSAGE( "base58 string" );
   {
      auto bs = koinos::util::from_base58< std::string >( b58_str );

      BOOST_CHECK_EQUAL( bs, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::to_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< char >" );
   {
      auto bs = koinos::util::from_base58< std::vector< char > >( b58_str );

      std::string result;

      for ( auto& c : bs )
         result += c;

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::to_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< unsigned char >" );
   {
      auto bs = koinos::util::from_base58< std::vector< unsigned char > >( b58_str );

      std::string result;

      for ( auto& c : bs )
         result += static_cast< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::to_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< std::byte >" );
   {
      auto bs = koinos::util::from_base58< std::vector< std::byte > >( b58_str );

      std::string result;

      for ( auto& c : bs )
         result += std::to_integer< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::to_base58( bs ) );
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

   BOOST_TEST_MESSAGE( "base58 generic type" );
   {
      auto b58_str = "1131SRtpx1";
      auto t = koinos::util::from_base58< uint64_t >( b58_str );

      BOOST_CHECK_EQUAL( t, 0x04080f10172aull );

      BOOST_CHECK_EQUAL( b58_str, koinos::util::to_base58( t ) );
   }
}

BOOST_AUTO_TEST_CASE( base64_test )
{
   // Test cases from https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/test

   BOOST_TEST_MESSAGE( "Basic Test" );

   const std::string orig =
      "René Nyffenegger\n"
      "http://www.renenyffenegger.ch\n"
      "passion for data\n";

   auto encoded = koinos::util::to_base64( orig );
   auto decoded = koinos::util::from_base64< std::string >( encoded );

   BOOST_CHECK_EQUAL( encoded, "UmVuw6kgTnlmZmVuZWdnZXIKaHR0cDovL3d3dy5yZW5lbnlmZmVuZWdnZXIuY2gKcGFzc2lvbiBmb3IgZGF0YQo=" );
   BOOST_CHECK_EQUAL( decoded, orig );

   BOOST_TEST_MESSAGE( "Test all possibilities of fill bytes (none, one =, two ==)" );

   std::string original = "abc";
   std::string reference = "YWJj";

   encoded = koinos::util::to_base64( original );
   decoded = koinos::util::from_base64< std::string >( encoded );

   BOOST_CHECK_EQUAL( encoded, reference );
   BOOST_CHECK_EQUAL( decoded, original );

   original = "abcd";
   reference = "YWJjZA==";

   encoded = koinos::util::to_base64( original );
   decoded = koinos::util::from_base64< std::string >( encoded );

   BOOST_CHECK_EQUAL( encoded, reference );
   BOOST_CHECK_EQUAL( decoded, original );

   original = "abcde";
   reference = "YWJjZGU=";

   encoded = koinos::util::to_base64( original );
   decoded = koinos::util::from_base64< std::string >( encoded );

   BOOST_CHECK_EQUAL( encoded, reference );
   BOOST_CHECK_EQUAL( decoded, original );

   BOOST_TEST_MESSAGE( "URL based tests" );

   original = "aaaaaaaaaaaaaaaaa";
   std::string expected = "YWFhYWFhYWFhYWFhYWFhYWE=";
   std::string expected_url = "YWFhYWFhYWFhYWFhYWFhYWE=";

   encoded = koinos::util::to_base64( original, false );
   auto encoded_url = koinos::util::to_base64( original );

   BOOST_CHECK_EQUAL( encoded, expected );
   BOOST_CHECK_EQUAL( encoded_url, expected_url );

   decoded = koinos::util::from_base64< std::string >( encoded );
   BOOST_CHECK_EQUAL( decoded, original );

   decoded = koinos::util::from_base64< std::string >( encoded_url );
   BOOST_CHECK_EQUAL( decoded, original );

   original = "\x03" "\xef" "\xff" "\xf9";
   expected = "A+//+Q==";
   expected_url = "A-__-Q==";

   encoded = koinos::util::to_base64( original, false );
   encoded_url = koinos::util::to_base64( original );

   BOOST_CHECK_EQUAL( encoded, expected );
   BOOST_CHECK_EQUAL( encoded_url, expected_url );

   decoded = koinos::util::from_base64< std::string >( encoded );
   BOOST_CHECK_EQUAL( decoded, original );

   decoded = koinos::util::from_base64< std::string >( encoded_url );
   BOOST_CHECK_EQUAL( decoded, original );

   BOOST_TEST_MESSAGE( "Test decoding without padding" );

   std::string input = "YWJjZGVmZw"; // Note the missing ==
   expected = "abcdefg";

   decoded = koinos::util::from_base64< std::string >( input );
   BOOST_CHECK_EQUAL( decoded, expected );

   input = "YWJjZGU"; // Note the missing =
   expected = "abcde";

   decoded = koinos::util::from_base64< std::string >( input );
   BOOST_CHECK_EQUAL( decoded, expected );

   input = "";
   expected = "";

   decoded = koinos::util::from_base64< std::string >( input );
   BOOST_CHECK_EQUAL( decoded, expected );

   input = "YQ";
   expected = "a";

   decoded = koinos::util::from_base64< std::string >( input );
   BOOST_CHECK_EQUAL( decoded, expected );

   input = "YWI";
   expected = "ab";

   decoded = koinos::util::from_base64< std::string >( input );
   BOOST_CHECK_EQUAL( decoded, expected );
}

BOOST_AUTO_TEST_CASE( conversion_test )
{
   uint32_t x = 10;

   auto s = koinos::util::converter::as< std::string >( x );
   auto y = koinos::util::converter::to< uint32_t >( s );

   BOOST_REQUIRE( x == y );

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

   auto new_byte_str = koinos::util::from_hex< std::string >( hex_str );

   BOOST_CHECK( byte_str == new_byte_str );

   uint64_t x = 0x04080f10172a;
   hex_str = koinos::util::to_hex( x );

   BOOST_CHECK( hex_str == "0x000004080f10172a" );
   uint64_t y = koinos::util::from_hex< uint64_t >( hex_str );

   BOOST_CHECK( x == y );
}

BOOST_AUTO_TEST_CASE( options_test )
{
   boost::program_options::variables_map cli_args;
   YAML::Node service_config;
   YAML::Node global_config;

   const char* args[] = {"test"};
   boost::program_options::options_description options;
   options.add_options()
      ("foo", boost::program_options::value< uint32_t >(), "test option" )
      ("bar", boost::program_options::bool_switch()->default_value(false), "test flag");
   boost::program_options::store( boost::program_options::parse_command_line( 1, args, options ), cli_args );

   uint32_t default_value = 0;

   auto i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == default_value );

   global_config[ "foo" ] = 1;
   global_config[ "bar" ] = true;
   i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == global_config[ "foo" ].as< uint32_t >() );

   service_config[ "foo" ] = 2;
   service_config[ "bar" ] = false;
   i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == service_config[ "foo" ].as< uint32_t >() );

   const char* args2[] = {"test", "--foo", "2", "--bar"};

   boost::program_options::store( boost::program_options::parse_command_line( 4, args2, options ), cli_args );
   i = koinos::util::get_option( "foo", default_value, cli_args, service_config, global_config );
   BOOST_CHECK( i == cli_args[ "foo" ].as< uint32_t >() );
}

BOOST_AUTO_TEST_SUITE_END()
