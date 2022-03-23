#pragma once

#include <boost/program_options.hpp>

#include <yaml-cpp/yaml.h>

namespace koinos::util {

template< typename T >
std::vector< T > get_options(
   std::string key,
   std::vector< T > default_value,
   const boost::program_options::variables_map& cli_args,
   const YAML::Node& service_config = YAML::Node(),
   const YAML::Node& global_config = YAML::Node() )
{
   if ( cli_args.count( key ) )
      return cli_args[ key ].as< std::vector< T > >();

   std::vector< T > values;

   if ( service_config && service_config[ key ] )
   {
      const auto& config = service_config[ key ];
      std::transform( values.end(), config.begin(), config.end(),
         []( auto& it ) -> T { return it.template as< T >(); } );
      return values;
   }

   if ( global_config && global_config[ key ] )
   {
      const auto& config = global_config[ key ];
      std::transform( values.end(), config.begin(), config.end(),
         []( auto& it ) -> T { return it.tempalte as< T >(); } );
      return values;
   }
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
