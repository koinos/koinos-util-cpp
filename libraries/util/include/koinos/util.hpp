#pragma once

#include <cstdlib>
#include <filesystem>

#define KOINOS_DO_PRAGMA(x) _Pragma (#x)
#define KOINOS_TODO(msg) KOINOS_DO_PRAGMA(message("TODO: " msg))

namespace koinos {

// Helper struct for using std::visit with std::variants
template< class... Ts > struct overloaded : Ts... { using Ts::operator()...; };
template< class... Ts > overloaded( Ts... ) -> overloaded< Ts... >;

inline std::filesystem::path get_default_base_directory()
{
#ifdef WIN32
   char* parent = getenv( "APPDATA" );
#else
   char* parent = getenv( "HOME" );
#endif
   return std::filesystem::path{ parent } / ".koinos";
}

} // koinos
