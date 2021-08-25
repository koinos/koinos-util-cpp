#include <boost/test/unit_test.hpp>

#include <koinos/bigint.hpp>

#include <iostream>

struct bigint_fixture {};

#define REQUIRE_DEEP_EQUAL( s, v )                         \
do{                                                        \
   BOOST_REQUIRE_EQUAL( s.tellp() - s.tellg(), v.size() ); \
   std::stringstream copy( s.str() );                      \
   for( size_t i = 0; i < v.size(); i++ )                  \
   {                                                       \
      uint8_t c;                                           \
      copy.get( (char&)c );                                \
      BOOST_CHECK_EQUAL( v[i], c );                        \
   }                                                       \
} while(0);

BOOST_FIXTURE_TEST_SUITE( bigint_tests, bigint_fixture )

BOOST_AUTO_TEST_CASE( int128_test )
{
   koinos::int128_t to_bin = 1;
   to_bin <<= 65;
   to_bin -= 1; // 2^65 - 1

   std::stringstream ss;
   koinos::to_binary( ss, to_bin );

   std::vector< uint8_t > expected = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   };
   REQUIRE_DEEP_EQUAL( ss, expected );

   koinos::int128_t from_bin;
   koinos::from_binary( ss, from_bin );
   BOOST_REQUIRE_EQUAL( from_bin, to_bin );
}

BOOST_AUTO_TEST_CASE( uint128_test )
{
   koinos::uint128_t to_bin = 1;
   to_bin <<= 65;
   to_bin -= 1; // 2^65 - 1

   std::stringstream ss;
   koinos::to_binary( ss, to_bin );

   std::vector< uint8_t > expected = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   };
   REQUIRE_DEEP_EQUAL( ss, expected );

   koinos::uint128_t from_bin;
   koinos::from_binary( ss, from_bin );
   BOOST_REQUIRE_EQUAL( from_bin, to_bin );
}

BOOST_AUTO_TEST_CASE( int160_test )
{
   koinos::int160_t to_bin = 1;
   to_bin <<= 129;
   to_bin -= 1; // 2^129 - 1

   std::stringstream ss;
   koinos::to_binary( ss, to_bin );

   std::vector< uint8_t > expected = {
      0x00, 0x00, 0x00, 0x01,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
   };
   REQUIRE_DEEP_EQUAL( ss, expected );

   koinos::int160_t from_bin;
   koinos::from_binary( ss, from_bin );
   BOOST_REQUIRE_EQUAL( from_bin, to_bin );
}

BOOST_AUTO_TEST_CASE( uint160_test )
{
   koinos::uint160_t to_bin = 1;
   to_bin <<= 129;
   to_bin -= 1; // 2^129 - 1

   std::stringstream ss;
   koinos::to_binary( ss, to_bin );

   std::vector< uint8_t > expected = {
      0x00, 0x00, 0x00, 0x01,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
   };
   REQUIRE_DEEP_EQUAL( ss, expected );

   koinos::uint160_t from_bin;
   koinos::from_binary( ss, from_bin );
   BOOST_REQUIRE_EQUAL( from_bin, to_bin );
}

BOOST_AUTO_TEST_CASE( int256_test )
{
   koinos::int256_t to_bin = 1;
   to_bin <<= 129;
   to_bin -= 1; // 2^129 - 1

   std::stringstream ss;
   koinos::to_binary( ss, to_bin );

   std::vector< uint8_t > expected = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
   };
   REQUIRE_DEEP_EQUAL( ss, expected );

   koinos::int256_t from_bin;
   koinos::from_binary( ss, from_bin );
   BOOST_REQUIRE_EQUAL( from_bin, to_bin );
}

BOOST_AUTO_TEST_CASE( uint256_test )
{
   koinos::uint256_t to_bin = 1;
   to_bin <<= 129;
   to_bin -= 1; // 2^129 - 1

   std::stringstream ss;
   koinos::to_binary( ss, to_bin );

   std::vector< uint8_t > expected = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
   };
   REQUIRE_DEEP_EQUAL( ss, expected );

   koinos::uint256_t from_bin;
   koinos::from_binary( ss, from_bin );
   BOOST_REQUIRE_EQUAL( from_bin, to_bin );
}

BOOST_AUTO_TEST_SUITE_END()
