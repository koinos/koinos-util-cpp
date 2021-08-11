#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <koinos/tests/util_fixture.hpp>
#include <koinos/base58.hpp>


BOOST_FIXTURE_TEST_SUITE( util_tests, util_fixture )

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

BOOST_AUTO_TEST_SUITE_END()
