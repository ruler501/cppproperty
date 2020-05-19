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

option(CPPPROPERTY_TESTS, "Include tests as a target for CppProperty." ON)

if(CPPPROPERTY_TESTS)
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
endif(CPPPROPERTY_TESTS)

option(CPPPROPERTY_EXAMPLES "Include examples as a target for CppProperty." ON)

if(CPPPROPERTY_EXAMPLES)
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples)
endif(CPPPROPERTY_TESTS)