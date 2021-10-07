#pragma once

#include <filesystem>

namespace koinos::util {

namespace service {
   constexpr char chain[]             = "chain";
   constexpr char block_store[]       = "block_store";
   constexpr char mempool[]           = "mempool";
   constexpr char block_producer[]    = "block_producer";
   constexpr char transaction_store[] = "transaction_store";
   constexpr char p2p[]               = "p2p";
}

std::filesystem::path get_default_base_directory();

} // koinos::util
