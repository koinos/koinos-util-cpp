#pragma once

#include <chrono>
#include <string>

namespace koinos::util {

template< class Rep, class Period >
std::string format_duration( const std::chrono::duration< Rep, Period >& d )
{
  std::stringstream ss;
  auto count = d.count();

  auto seconds  = count % 60;
  count        /= 60;
  auto minutes  = count % 60;
  count        /= 60;
  auto hours    = count % 24;
  count        /= 24;
  auto days     = count % 365;
  auto years    = count / 365;

  if( years )
  {
    ss << years << "y, " << days << "d, ";
  }
  else if( days )
  {
    ss << days << "d, ";
  }

  ss << std::setw( 2 ) << std::setfill( '0' ) << hours;
  ss << std::setw( 1 ) << "h, ";
  ss << std::setw( 2 ) << std::setfill( '0' ) << minutes;
  ss << std::setw( 1 ) << "m, ";
  ss << std::setw( 2 ) << std::setfill( '0' ) << seconds;
  ss << std::setw( 1 ) << "s";
  return ss.str();
}

} // namespace koinos::util
