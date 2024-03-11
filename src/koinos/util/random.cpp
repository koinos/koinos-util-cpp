#include <koinos/util/random.hpp>

#include <algorithm>

namespace koinos::util {

std::string random_alphanumeric( std::size_t len )
{
  thread_local std::mt19937 generator( std::random_device{}() );
  auto random_char = [ & ]() -> char
  {
    constexpr char charset[]        = "0123456789"
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

} // namespace koinos::util
