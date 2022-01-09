cmake_minimum_required(VERSION 3.15)

# Set the project name and standard settings
project(cpp_lru_cache CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Interface which specifies the project's options
add_library(project_options INTERFACE)

# Interface which specifies the project's warnings
add_library(project_warnings INTERFACE)

# Set C++11 standard
target_compile_features(project_options INTERFACE cxx_std_11)

# Add linker configuration
include(cmake/Linker.cmake)
configure_linker(project_options)

# Add compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Add static analyzers
include(cmake/StaticAnalyzers.cmake)

# Add sanitizers configuration
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# Add project specific directories
include_directories(include)

# Add testing directory
enable_testing()
add_subdirectory(test)
