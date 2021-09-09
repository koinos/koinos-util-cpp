#include <koinos/util.hpp>

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace koinos {

std::filesystem::path get_default_base_directory()
{
#ifdef WIN32
   char* parent = getenv( "APPDATA" );
#else
   char* parent = getenv( "HOME" );
#endif
   return std::filesystem::path{ parent } / ".koinos";
}

std::string random_alphanumeric( std::size_t len )
{
   thread_local std::mt19937 generator( std::random_device{}() );
   auto random_char = [&]() -> char
   {
      constexpr char charset[] =
         "0123456789"
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         "abcdefghijklmnopqrstuvwxyz";
      constexpr std::size_t max_index = sizeof( charset ) - 1;
      std::uniform_int_distribution<> distribution( 0, max_index );
      return charset[ distribution( generator ) % max_index ];
   };
   std::string str( len, 0 );
   std::generate_n( str.begin(), len, random_char );
   return str;
}

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

} // koinos

