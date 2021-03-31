#pragma once

#include <filesystem>
#include <random>

#define KOINOS_DO_PRAGMA(x) _Pragma (#x)
#define KOINOS_TODO(msg) KOINOS_DO_PRAGMA(message("TODO: " msg))

namespace koinos {

namespace service {
   constexpr char chain[]          = "chain";
   constexpr char block_store[]    = "block_store";
   constexpr char mempool[]        = "mempool";
   constexpr char block_producer[] = "block_producer";
   constexpr char p2p[]            = "p2p";
}

// Helper struct for using std::visit with std::variants
template< class... Ts > struct overloaded : Ts... { using Ts::operator()...; };
template< class... Ts > overloaded( Ts... ) -> overloaded< Ts... >;

std::filesystem::path get_default_base_directory();

std::string random_alphanumeric( std::size_t len );

} // koinos
