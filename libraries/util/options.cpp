#include <koinos/util/options.hpp>

namespace koinos::util {

bool get_flag(
   std::string key,
   bool default_value,
   const boost::program_options::variables_map& cli_args,
   const YAML::Node& service_config,
   const YAML::Node& global_config )
{
   if ( cli_args.count( key ) && cli_args[ key ].as< bool >() != default_value )
      return cli_args[ key ].as< bool >();

   if ( service_config && service_config[ key ] )
      return service_config[ key ].as< bool >();

   if ( global_config && global_config[ key ] )
      return global_config[ key ].as< bool >();

   return default_value;
}

} // koinos::util
