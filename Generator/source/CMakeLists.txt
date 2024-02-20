# Set minimum version of cmake
cmake_minimum_required(VERSION 3.13)

# Define project name, version and description
project(CircuitsGenerator
  VERSION 0.1.0
  DESCRIPTION "Program for circuits dataset generation"
  HOMEPAGE_URL https://github.com/vvzunin/CAD_CombinationalCircuits
  LANGUAGES CXX
)

# Installing Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Конфигурация файла Doxyfile
    set(DOXYGEN_INPUT ${\Generator\source}/src) # The path to the source files of your project
    set(DOXYGEN_OUTPUT_DIRECTORY ${\Generator}/doc) # The path to save the documentation
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN NO)
    set(DOXYGEN_EXTRACT_ALL YES)

    # Генерация Doxyfile
    configure_file(Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

    # Создание цели сборки для генерации документации
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()

# Add path for .cmake files. Needed for "include" directive.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Auxiliary variable definitions
# Define variables for coverage
set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov")

# Define main cmake project settings
set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")
set(CMAKE_BUILD_TYPE Debug)

# Define folder for external libraries
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# Define names of standalone binary and library.
set(MAIN_TARGET circuitgen)
set(MAIN_LIBRARY cgen)

# Needed to build library.
set(BUILD_LIBRARY True)

OPTION(GENERATOR_DEBUG "Extended debug printing" OFF)

# Configuration file.
configure_file(config.h.in config.h)

option(BUILD_SHARED_PROJECT_LIBS "Build using shared libraries" ON)

include(ExternalProject)
include(FetchContent)

# Get json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Get googletest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add all subdirectories for the project.
add_subdirectory(AuxiliaryMethods)
add_subdirectory(circuits)
add_subdirectory(DataBase)
add_subdirectory(filesTools)
add_subdirectory(generators)
add_subdirectory(graph)
add_subdirectory(optimization_utils)
add_subdirectory(reliability)
add_subdirectory(settings)
add_subdirectory(thread_pool)

add_subdirectory(tests)

# Set all libraries, using in project.
set(PROJECT_LIBS

  # Internal libraries
  AuxiliaryMethods
  Circuit
  DataBaseGenerator
  FilesTools
  GraphVertex
  OptimizationUtils
  OrientedGraph
  Reliability
  Settings
  ThreadPool
  TruthTable

  # External libraries
  nlohmann_json::nlohmann_json
)

# Add source files
set(SOURCES
  AuxiliaryMethods/AuxiliaryMethods.cpp
  circuits/Circuit.cpp
  DataBase/DataBaseGenerator.cpp
  DataBase/DataBaseGeneratorParameters.cpp
  filesTools/FilesTools.cpp
  generators/Genetic/Mutations/MutationParameters.cpp
  generators/Genetic/Parents/ParentsParameters.cpp
  generators/Genetic/Recombination/RecombinationParameters.cpp
  generators/Genetic/Selections/SelectionParameters.cpp
  generators/Genetic/Chronosome.cpp
  generators/Genetic/ChronosomeType.cpp
  generators/Genetic/GeneticParameters.cpp
  generators/Genetic/GenGenerator.cpp
  generators/GenerationParameters.cpp
  generators/Parser.cpp
  generators/SimpleGenerators.cpp
  generators/TruthTable.cpp
  graph/GraphVertex.cpp
  graph/OrientedGraph.cpp
  optimization_utils/AbcUtils.cpp
  optimization_utils/YosysUtils.cpp
  reliability/Reliability.cpp
  settings/Settings.cpp
  thread_pool/ThreadPool.cpp

  generatorAPI.cpp
  main.cpp
)

# Check if pthreads is installed; adjust compiler/linker options
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Create executable
add_executable(${MAIN_TARGET} ${SOURCES})

# Add all internal libraries in the project
target_link_libraries(${MAIN_TARGET} ${PROJECT_LIBS} Threads::Threads)




# -----------------------------------------------------------------------------
# Link to shared library
function(gen_properties target visibility)
  target_include_directories(${target} ${visibility} ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${target} ${visibility} ${PROJECT_LIBS})
endfunction()

add_library(${MAIN_LIBRARY} SHARED EXCLUDE_FROM_ALL ${SOURCES})
gen_properties(${MAIN_LIBRARY} PRIVATE)

set(public_headers
  generatorAPI.h
)

set_target_properties(${MAIN_LIBRARY} PROPERTIES PUBLIC_HEADER "${public_headers}")

set_property(TARGET ${MAIN_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${MAIN_LIBRARY} PROPERTY OUTPUT_NAME ${MAIN_LIBRARY})
