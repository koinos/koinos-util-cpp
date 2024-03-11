#pragma once

namespace koinos::util {

// Helper struct for using std::visit with std::variants
template< class... Ts >
struct overloaded: Ts...
{
  using Ts::operator()...;
};
template< class... Ts >
overloaded( Ts... ) -> overloaded< Ts... >;

} // namespace koinos::util
