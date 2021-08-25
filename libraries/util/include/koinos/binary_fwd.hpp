#pragma once

//#include <iostream>

namespace koinos {

template< typename T = void >
inline void to_binary( std::ostream&, const T& ) = delete;

template< typename T = void >
inline void from_binary( std::istream&, T& ) = delete;

} // koinos
