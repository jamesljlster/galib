cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(GA_TYPE "double" CACHE STRING "GA variable type")
set(GA_PREFIX "ga" CACHE STRING "GA function prefix")

# Check if required information is set or not
if(NOT GA_TYPE)
	message("GA_TYPE is not set!")
endif()

if(NOT GA_PREFIX)
	message("GA function prefix is not set")
endif()

# Generate files with cmake setting
file(GLOB PROJECT_CFG_IN ${CMAKE_CURRENT_SOURCE_DIR}/*.in)
foreach(CFG_IN_PATH ${PROJECT_CFG_IN})
	# Get each file name
	get_filename_component(CFG_IN_FILENAME ${CFG_IN_PATH} NAME)
	STRING(REGEX REPLACE "\\.[^.]*$" "" CFG_IN_NAME ${CFG_IN_FILENAME})

	# Configure files
	configure_file(${CFG_IN_FILENAME} ${CMAKE_CURRENT_SOURCE_DIR}/${CFG_IN_NAME})

endforeach()

# Variables
set(PROJECT_NAME galib)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build confugure" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Set project
project(${PROJECT_NAME} C CXX)

# Add definition
if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DDEBUG)
endif()

# Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set file list
file(GLOB PROJECT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(PROJECT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/galib.h)

# Build library
add_library(${PROJECT_NAME} STATIC ${PROJECT_SRCS})
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	OUTPUT_NAME			${PROJECT_NAME}
		PREFIX			"lib"
		PUBLIC_HEADER	${PROJECT_HEADER}
	)

# Install
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
	LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
	)

