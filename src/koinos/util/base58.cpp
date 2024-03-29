#include <koinos/util/base58.hpp>

namespace koinos::util {

/** All alphanumeric characters except for "0", "I", "O", and "l" */
static const char* pszBase58         = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
static const int8_t mapBase58[ 256 ] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
  -1, -1, -1, -1, -1, -1, -1, 9,  10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1, 22, 23, 24, 25, 26, 27, 28,
  29, 30, 31, 32, -1, -1, -1, -1, -1, -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46, 47, 48, 49, 50,
  51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

constexpr bool is_space( char c ) noexcept
{
  return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

bool decode_base58( const char* psz, std::vector< char >& dest, std::size_t max_ret_len )
{
  // Skip leading spaces.
  while( *psz && is_space( *psz ) )
    psz++;
  // Skip and count leading '1's.
  int zeroes = 0;
  int length = 0;
  while( *psz == '1' )
  {
    zeroes++;
    if( zeroes > max_ret_len )
      return false;
    psz++;
  }
  // Allocate enough space in big-endian base256 representation.
  int size = strlen( psz ) * 733 / 1'000 + 1; // log(58) / log(256), rounded up.
  std::vector< unsigned char > b256( size );
  // Process the characters.
  static_assert( sizeof( mapBase58 ) / sizeof( mapBase58[ 0 ] ) == 256,
                 "mapBase58.size() should be 256" ); // guarantee not out of range
  while( *psz && !is_space( *psz ) )
  {
    // Decode base58 character
    int carry = mapBase58[ (uint8_t)*psz ];
    if( carry == -1 ) // Invalid b58 character
      return false;
    int i = 0;
    for( std::vector< unsigned char >::reverse_iterator it = b256.rbegin();
         ( carry != 0 || i < length ) && ( it != b256.rend() );
         ++it, ++i )
    {
      carry += 58 * ( *it );
      *it    = carry % 256;
      carry /= 256;
    }
    assert( carry == 0 );
    length = i;
    if( length + zeroes > max_ret_len )
      return false;
    psz++;
  }
  // Skip trailing spaces.
  while( is_space( *psz ) )
    psz++;
  if( *psz != 0 )
    return false;
  // Skip leading zeroes in b256.
  std::vector< unsigned char >::iterator it = b256.begin() + ( size - length );
  // Copy result into output vector.
  dest.reserve( zeroes + ( b256.end() - it ) );
  dest.assign( zeroes, 0x00 );
  while( it != b256.end() )
    dest.push_back( *( it++ ) );
  return true;
}

std::string encode_base58( const unsigned char* pbegin, const unsigned char* pend )
{
  // Skip & count leading zeroes.
  int zeroes = 0;
  int length = 0;
  while( pbegin != pend && *pbegin == 0 )
  {
    pbegin++;
    zeroes++;
  }
  // Allocate enough space in big-endian base58 representation.
  int size = ( pend - pbegin ) * 138 / 100 + 1; // log(256) / log(58), rounded up.
  std::vector< unsigned char > b58( size );
  // Process the bytes.
  while( pbegin != pend )
  {
    int carry = *pbegin;
    int i     = 0;
    // Apply "b58 = b58 * 256 + ch".
    for( std::vector< unsigned char >::reverse_iterator it = b58.rbegin();
         ( carry != 0 || i < length ) && ( it != b58.rend() );
         it++, i++ )
    {
      carry += 256 * ( *it );
      *it    = carry % 58;
      carry /= 58;
    }

    assert( carry == 0 );
    length = i;
    pbegin++;
  }
  // Skip leading zeroes in base58 result.
  std::vector< unsigned char >::iterator it = b58.begin() + ( size - length );
  while( it != b58.end() && *it == 0 )
    it++;
  // Translate the result into a string.
  std::string str;
  str.reserve( zeroes + ( b58.end() - it ) );
  str.assign( zeroes, '1' );
  while( it != b58.end() )
    str += pszBase58[ *( it++ ) ];
  return str;
}

void decode_base58( const std::string& src, std::vector< char >& dest )
{
  decode_base58( src.data(), src.size(), dest );
}

void decode_base58( const std::string& src, std::vector< unsigned char >& dest )
{
  std::vector< char > v;
  decode_base58( src.data(), src.size(), v );
  dest.assign( v.begin(), v.end() );
}

void decode_base58( const std::string& src, std::vector< std::byte >& dest )
{
  std::vector< char > v;
  decode_base58( src.data(), src.size(), v );
  auto* begin = reinterpret_cast< std::byte* >( v.data() );
  dest.assign( begin, begin + v.size() );
}

void encode_base58( std::string& s, const std::string& v )
{
  unsigned char* begin = (unsigned char*)v.c_str();
  s                    = encode_base58( begin, begin + v.size() );
}

void decode_base58( const char* begin, size_t count, std::vector< char >& dest )
{
  std::vector< char > temp( begin, begin + count );
  temp.push_back( '\0' );
  if( !decode_base58( temp.data(), dest ) )
  {
    throw std::runtime_error( "base58 decoding unsuccessful" );
  }
}

void encode_base58( const char* begin, size_t count, std::vector< char >& dest )
{
  std::string temp = encode_base58( (const unsigned char*)begin, (const unsigned char*)begin + count );
  dest.resize( temp.size() );
  std::copy( temp.begin(), temp.end(), dest.begin() );
}

std::string encode_base58( const std::vector< char >& v )
{
  const unsigned char* begin = reinterpret_cast< const unsigned char* >( v.data() );
  return encode_base58( begin, begin + v.size() );
}

std::string encode_base58( const std::vector< unsigned char >& v )
{
  const unsigned char* begin = v.data();
  return encode_base58( begin, begin + v.size() );
}

std::string encode_base58( const std::vector< std::byte >& v )
{
  const unsigned char* begin = reinterpret_cast< const unsigned char* >( v.data() );
  return encode_base58( begin, begin + v.size() );
}

template<>
std::string to_base58< std::string >( const std::string& s )
{
  return encode_base58( reinterpret_cast< const unsigned char* >( s.c_str() ),
                        reinterpret_cast< const unsigned char* >( s.c_str() + s.size() ) );
}

template<>
std::string from_base58< std::string >( const std::string& s )
{
  std::vector< char > v;
  if( !decode_base58( s.c_str(), v ) )
  {
    throw std::runtime_error( "base58 decoding unsuccessful" );
  }

  return converter::as< std::string >( v );
}

template<>
std::string to_base58< std::vector< char > >( const std::vector< char >& v )
{
  return encode_base58( v );
}

template<>
std::vector< char > from_base58< std::vector< char > >( const std::string& s )
{
  std::vector< char > v;
  decode_base58( s, v );
  return v;
}

template<>
std::string to_base58< std::vector< unsigned char > >( const std::vector< unsigned char >& v )
{
  return encode_base58( v );
}

template<>
std::vector< unsigned char > from_base58< std::vector< unsigned char > >( const std::string& s )
{
  std::vector< unsigned char > v;
  decode_base58( s, v );
  return v;
}

template<>
std::string to_base58< std::vector< std::byte > >( const std::vector< std::byte >& v )
{
  return encode_base58( v );
}

template<>
std::vector< std::byte > from_base58< std::vector< std::byte > >( const std::string& s )
{
  std::vector< std::byte > v;
  decode_base58( s, v );
  return v;
}

} // namespace koinos::util
