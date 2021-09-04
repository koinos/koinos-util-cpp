#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <koinos/tests/util_fixture.hpp>
#include <koinos/base58.hpp>
#include <koinos/binary.hpp>
#include <koinos/conversion.hpp>
#include <koinos/varint.hpp>

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
      koinos::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += c;

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< unsigned char >" );
   {
      std::vector< unsigned char > bs;
      koinos::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += static_cast< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::vector< std::byte >" );
   {
      std::vector< std::byte > bs;
      koinos::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += std::to_integer< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< char, N >" );
   {
      std::array< char, array_size > bs;
      koinos::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += c;

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< unsigned char, N >" );
   {
      std::array< unsigned char, array_size > bs;
      koinos::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += static_cast< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< std::byte, N >" );
   {
      std::array< std::byte, array_size > bs;
      koinos::decode_base58( b58_str, bs );

      std::string result;

      for ( auto& c : bs )
         result += std::to_integer< char >( c );

      BOOST_CHECK_EQUAL( result, str );

      BOOST_CHECK_EQUAL( b58_str, koinos::encode_base58( bs ) );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< char, N > overflow" );
   {
      std::array< char, array_size_overflow > bs;
      BOOST_CHECK_THROW( koinos::decode_base58( b58_str, bs ), std::runtime_error );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< unsigned char, N > overflow" );
   {
      std::array< unsigned char, array_size_overflow > bs;
      BOOST_CHECK_THROW( koinos::decode_base58( b58_str, bs ), std::runtime_error );
   }

   BOOST_TEST_MESSAGE( "base58 std::array< std::byte, N > overflow" );
   {
      std::array< std::byte, array_size_overflow > bs;
      BOOST_CHECK_THROW( koinos::decode_base58( b58_str, bs ), std::runtime_error );
   }
}

BOOST_AUTO_TEST_CASE( conversion_test )
{
   uint32_t x = 10;

   auto s = koinos::converter::as< std::string >( x );
   auto y = koinos::converter::to< uint32_t >( s );

   BOOST_REQUIRE( x == y );

   y = 20;
   uint64_t z = 30;

   s = koinos::converter::as< std::string >( x, z );

   auto [a, b] = koinos::converter::to< uint32_t, uint64_t >( s );

   BOOST_REQUIRE( x == a );
   BOOST_REQUIRE( z == b );

   auto t = koinos::converter::as< std::string >( z );

   s = koinos::converter::as< std::string >( x, t );

   auto [c, d] = koinos::converter::to< uint64_t, uint32_t >( s );

   BOOST_REQUIRE( c == uint64_t( x ) << 32 );
   BOOST_REQUIRE( d == uint32_t( z ) );
}

BOOST_AUTO_TEST_SUITE_END()
