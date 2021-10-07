#include <koinos/util/hex.hpp>

#include <iomanip>
#include <sstream>

namespace koinos::util {

std::string to_hex( const std::string& s )
{
   std::stringstream stream;
   stream << "0x" << std::hex << std::setfill( '0' );
   for ( const auto& c : s )
   {
      stream << std::setw( 2 ) << static_cast< unsigned int >( static_cast< unsigned char >( c ) );
   }
   return stream.str();
}

char hex_to_char( char in )
{
   if ( in >= '0' && in <= '9' )
      return in - '0';
   if ( in >= 'a' && in <= 'f' )
      return in - 'a' + 10;
   if ( in >= 'A' && in <= 'F' )
      return in - 'A' + 10;
   throw std::logic_error( "input is not hex" );
}

std::string from_hex( const std::string& s )
{
   std::string bytes;
   bytes.reserve( s.size() / 2 );
   std::stringstream ss( s );
   auto data = s.c_str();

   for ( size_t i = data[0] == '0' && data[1] == 'x' ? 2 : 0;
         i < s.size();
         i += 2 )
   {
      bytes.push_back( hex_to_char( data[i] ) << 4 | hex_to_char( data[i + 1] ) );
   }

   return bytes;
}

} // koinos::util
