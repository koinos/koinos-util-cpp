#include <koinos/util/base64.hpp>

#include "base64.h"

#include <sstream>

namespace koinos::util {

template<>
std::string to_base64< std::string >( const std::string& s, bool websafe )
{
   return base64_encode( s, websafe );
}

template<>
std::string from_base64< std::string >( const std::string& s )
{
   return base64_decode( s );
}

} // koinos::util
