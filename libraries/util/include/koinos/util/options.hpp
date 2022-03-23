#pragma once

#include <boost/program_options.hpp>

#include <yaml-cpp/yaml.h>

namespace koinos::util {

template< typename T >
std::vector< T > get_options(
   std::string key,
   const boost::program_options::variables_map& cli_args,
   const YAML::Node& service_config = YAML::Node(),
   const YAML::Node& global_config = YAML::Node() )
{
   std::vector< T > values;

   if ( cli_args.count( key ) )
   {
      values = cli_args[ key ].as< std::vector< T > >();
   }

   if ( service_config && service_config[ key ] )
   {
      for ( auto& n : service_config[ key ] )
         values.emplace_back( n.template as< T >() );
   }

   if ( global_config && global_config[ key ] )
   {
      for ( auto& n : global_config[ key ] )
         values.emplace_back( n.template as< T >() );
   }

   return std::move( values );
}

template< typename T >
T get_option(
   std::string key,
   T default_value,
   const boost::program_options::variables_map& cli_args,
   const YAML::Node& service_config = YAML::Node(),
   const YAML::Node& global_config = YAML::Node() )
{
   if ( cli_args.count( key ) )
      return cli_args[ key ].as< T >();

   if ( service_config && service_config[ key ] )
      return service_config[ key ].as< T >();

   if ( global_config && global_config[ key ] )
      return global_config[ key ].as< T >();

   return std::move( default_value );
}

} // koinos::util
