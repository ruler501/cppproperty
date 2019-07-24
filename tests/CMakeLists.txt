include(GoogleTest)

project (CppPropertyTest)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

if(MSVC)
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(MSVC)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Setup the test and configure it to use GoogleTest
# Need to include the header files to make visual studio happy
add_executable (CppPropertyTest "tests/property.cpp")

target_link_libraries (CppPropertyTest CppProperty gtest_main)

target_compile_features(CppPropertyTest PRIVATE cxx_std_20)

if(MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else (MSVC)
    if(CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tests")
		include(CodeCoverage)
		SETUP_TARGET_FOR_COVERAGE(CppPropertyCoverage CppPropertyTest coverage)
	endif(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(CppPropertyTest PRIVATE -Wall -Wextra -pedantic ${CMAKE_CXX_FLAGS_COVERAGE})
endif(MSVC)

enable_testing ()

gtest_discover_tests (CppPropertyTest)