#pragma once

#include <boost/endian/conversion.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <koinos/binary.hpp>

namespace koinos {

using int128_t  = boost::multiprecision::int128_t;
using uint128_t = boost::multiprecision::uint128_t;
using int256_t  = boost::multiprecision::int256_t;
using uint256_t = boost::multiprecision::uint256_t;

using int160_t = boost::multiprecision::number<
  boost::multiprecision::
    cpp_int_backend< 160, 160, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void > >;

using uint160_t = boost::multiprecision::number<
  boost::multiprecision::
    cpp_int_backend< 160, 160, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void > >;

template<>
void to_binary< int128_t >( std::ostream& s, const int128_t& v );

template<>
void from_binary< int128_t >( std::istream& s, int128_t& v );

template<>
void to_binary< uint128_t >( std::ostream& s, const uint128_t& v );

template<>
void from_binary< uint128_t >( std::istream& s, uint128_t& v );

template<>
void to_binary< int160_t >( std::ostream& s, const int160_t& v );

template<>
void from_binary< int160_t >( std::istream& s, int160_t& v );

template<>
void to_binary< uint160_t >( std::ostream& s, const uint160_t& v );

template<>
void from_binary< uint160_t >( std::istream& s, uint160_t& v );

template<>
void to_binary< int256_t >( std::ostream& s, const int256_t& v );

template<>
void from_binary< int256_t >( std::istream& s, int256_t& v );

template<>
void to_binary< uint256_t >( std::ostream& s, const uint256_t& v );

template<>
void from_binary< uint256_t >( std::istream& s, uint256_t& v );

} // namespace koinos
