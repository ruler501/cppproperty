cmake_minimum_required (VERSION 3.10)
project(CppPropertyExamples)

add_executable(CppPropertyExamples simpleUsage.cpp)

target_compile_features(CppPropertyExamples PRIVATE cxx_std_20)

target_link_libraries(CppPropertyExamples CppProperty)