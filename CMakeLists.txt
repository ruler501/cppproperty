cmake_minimum_required (VERSION 3.10)
project(CppProperty)

set(property_VERSION_MAJOR 0)
set(property_VERSION_MINOR 1)

add_library(CppProperty INTERFACE)
target_include_directories(CppProperty INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)
target_sources(CppProperty INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/cppproperty/property.h)

target_compile_features(CppProperty INTERFACE cxx_std_20)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(CppProperty INTERFACE -fconcepts)
endif(CMAKE_COMPILER_IS_GNUCXX)

option(CPPPROPERTY_TESTS, "Include tests as a target." ON)

if(CPPPROPERTY_TESTS)
	include(${CMAKE_CURRENT_LIST_DIR}/tests/CMakeLists.txt)
endif(CPPPROPERTY_TESTS)