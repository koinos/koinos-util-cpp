#pragma once

#include <cstdint>
#include <stdexcept>

#include <koinos/binary.hpp>

namespace koinos {

struct signed_varint
{
  signed_varint( int64_t v ):
      value( v )
  {}

  signed_varint() = default;
  int64_t value   = 0;
};

struct unsigned_varint
{
  unsigned_varint( uint64_t v ):
      value( v )
  {}

  unsigned_varint() = default;
  uint64_t value    = 0;
};

template<>
void to_binary< unsigned_varint >( std::ostream& s, const unsigned_varint& v );

template<>
void from_binary< unsigned_varint >( std::istream& s, unsigned_varint& v );

template<>
void to_binary< signed_varint >( std::ostream& s, const signed_varint& v );

template<>
void from_binary< signed_varint >( std::istream& s, signed_varint& v );

} // namespace koinos
