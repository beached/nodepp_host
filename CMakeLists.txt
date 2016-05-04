cmake_minimum_required( VERSION 2.8.4 )
include( ExternalProject )
project( nodepp_host )

set( Boost_USE_STATIC_LIBS OFF ) 
set( Boost_USE_MULTITHREADED ON )  
set( Boost_USE_STATIC_RUNTIME OFF ) 
find_package( Boost 1.59.0 REQUIRED COMPONENTS system filesystem regex unit_test_framework program_options iostreams )
set( Boost_Libs ${BOOST_Libraries} -lboost_system -lboost_regex -lboost_filesystem -lboost_program_options -lboost_iostreams )

MESSAGE( WARNING "Current Compiler:" ${CMAKE_CXX_COMPILER_ID} )
IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC" )
	MESSAGE( WARNING "Using Visual Studio" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601 /MP" )
	set( Boost_Libs )
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
	MESSAGE( WARNING "Using Clang" )
	find_package( Threads REQUIRED )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -g -O3 -Weverything -Wno-c++98-compat  -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion" )
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
	MESSAGE( WARNING "Using G++" )
	find_package( Threads REQUIRED )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O3 -Wall -Wno-deprecated-declarations" )
ENDIF( )

if( THREADS_HAVE_PTHREAD_ARG )
	set( COMPILER_SPECIFIC_LIBS, "-lpthread" )
endif( )

enable_testing( )

ExternalProject_Add( 
	nodepp
	GIT_REPOSITORY "https://github.com/beached/nodepp.git" 
	GIT_TAG "master"
	PATCH_COMMAND ""
	INSTALL_COMMAND ""
	SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/nodepp"
	TEST_COMMAND ""
 )

set( SOURCE_FILES
	nodepp_host.cpp
 )

include_directories( "${CMAKE_SOURCE_DIR}/3rdparty/nodepp/char_range" )
include_directories( "${CMAKE_SOURCE_DIR}/3rdparty/nodepp/header_libraries" )
include_directories( "${CMAKE_SOURCE_DIR}/3rdparty/nodepp/lib_nodepp" )
include_directories( "${CMAKE_SOURCE_DIR}/3rdparty/nodepp/load_library" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

link_directories( ${Boost_LIBRARY_DIRS} )

add_executable( nodepp_host ${SOURCE_FILES} )
target_link_libraries( nodepp_host ${CMAKE_DL_LIBS} ${Boost_Libs} ${CMAKE_THREAD_LIBS_INIT} ${COMPILER_SPECIFIC_LIBS} )