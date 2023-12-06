#include <koinos/util/services.hpp>

namespace koinos::util {

std::filesystem::path get_default_base_directory()
{
#ifdef WIN32
   char* parent = getenv( "APPDATA" );
#else
   char* parent = getenv( "HOME" );
#endif
   return std::filesystem::path{ parent } / ".koinos";
}

} // koinos::util
