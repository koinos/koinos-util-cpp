#pragma once

#include <type_traits>

#include <koinos/binary_fwd.hpp>

namespace koinos {

template< class... >
using void_t = int;

template< typename T, class = void_t<> >
struct has_to_binary : std::false_type{};

template< typename T >
struct has_to_binary< T, void_t< decltype( &to_binary< std::remove_const_t< std::remove_reference_t < T > > > ) > > : std::true_type{};

template< typename T >
constexpr auto has_to_binary_v = has_to_binary< T >::value;

} // koinos
