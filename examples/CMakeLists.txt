cmake_minimum_required (VERSION 3.10)
project(CppPropertyExamples)

add_executable(CppPropertyExamples simpleUsage.cpp)

target_compile_features(CppPropertyExamples PRIVATE cxx_std_20)
			  
include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

target_link_libraries(CppPropertyExamples CppProperty)