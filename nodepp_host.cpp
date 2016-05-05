// The MIT License (MIT)
//
// Copyright (c) 2014-2015 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <boost/program_options.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/utility/string_ref.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include "parse_json/daw_json.h"

struct ProgramOptions {
	std::string configuration_path;
};
	
ProgramOptions parse_command_line( int argc, char ** argv ) {
	namespace po = boost::program_options;

	ProgramOptions result;

	po::options_description desc("Allowed options");
	po::options_description po_desc("Positional options");
	desc.add_options( )
		( "help", "produce help message" );

	po_desc.add_options( )
		( "config_file", po::value<std::string>( &result.configuration_path ), "NodePP Host configuration file" );


	po::options_description all_options;
	all_options.add( desc );
	all_options.add( po_desc );

	po::positional_options_description positional_options; 
	positional_options.add( "config_file", 1 );

	po::variables_map vm;
	try {
		po::store( po::command_line_parser( argc, argv ).options( all_options ).positional( positional_options ).run( ), vm );

		if( vm.count( "help" ) || !vm.count( "config_file" ) ) {
			std::cout << argv[0] << " <config_file>\n" << desc << "\n";
			exit( EXIT_SUCCESS );
		}
		po::notify( vm ); 
	} catch( po::required_option const & roe ) {
		std::cerr << "Error: " << roe.what( ) << std::endl;
		exit( EXIT_FAILURE );
	} catch( po::error const & e ) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		exit( EXIT_FAILURE );
	}

	return result;
}

auto parse_config_file( boost::string_ref configuration_path ) {
	boost::iostreams::mapped_file_source configuration_file;
	configuration_file.open( configuration_path.to_string( ) );
	if( !configuration_file.is_open( ) ) {
		std::cerr << "Error opening NodePP Host configuration file: '" << configuration_path.to_string( ) << "'\n";
		exit( EXIT_FAILURE );
	}

	auto result = daw::json::parse_json( configuration_file.begin( ), configuration_file.end( ) );

	if( result.is_null( ) ) {
		std::cerr << "Error opening NodePP Host configuration file: '" << configuration_path.to_string( ) << "'\n";
		exit( EXIT_FAILURE );
	}
	
	return result;
}

int main( int argc, char ** argv ) {
	auto opts = parse_command_line( argc, argv );
	auto configuration_data = parse_config_file( opts.configuration_path );

	std::cout << configuration_data << std::endl;

	return EXIT_SUCCESS;
}

