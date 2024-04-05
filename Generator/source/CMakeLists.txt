cmake_minimum_required(VERSION 3.1)

project(Generator)

include(ExternalProject)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O0")
set(CMAKE_BUILD_TYPE Debug)


set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

#ExternalProject_Add(googletest
#  GIT_REPOSITORY https://github.com/google/googletest
#  GIT_TAG main
#  SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest-src"
#  BINARY_DIR "${CMAKE_BINARY_DIR}/googletest-build" 
#  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
#)

#add_subdirectory (${CMAKE_BINARY_DIR}/external/lib/cmake/GTest)

include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



# Add source files
set(SOURCES
  Settings.cpp
  AuxiliaryMethods.cpp
  FilesTools.cpp
  generators/Genetic/Parents/ParentsParameters.cpp
  generators/Genetic/Recombination/RecombinationParameters.cpp
  generators/Genetic/Recombination/RecombinationsTruthTable.cpp
  generators/Genetic/Mutations/MutationParameters.cpp
  generators/Genetic/Mutations/MutationTruthTable.cpp
  generators/Genetic/Mutations/Mutation.cpp
  generators/Genetic/Selections/SelectionParameters.cpp
  generators/Genetic/GeneticParameters.cpp
  generators/GenerationParameters.cpp
  graph/GraphVertex.cpp
  graph/OrientedGraph.cpp
  reliability/Reliability.cpp
  generators/TruthTable.cpp
  generators/Genetic/Chronosome.cpp
  generators/Genetic/ChronosomeType.cpp
  circuits/Circuit.cpp
  generators/Parser.cpp
  generators/SimpleGenerators.cpp
  generators/Genetic/GenGenerator.cpp
  DataBase/DataBaseGenerator.cpp
  DataBase/DataBaseGeneratorParameters.cpp
  main.cpp
)

# Add include directories
#include_directories(
#  /opt/homebrew/Cellar/nlohmann-json/3.11.2/include
#)

# Create executable
add_executable(prog ${SOURCES})

add_library(settings Settings.h Settings.cpp)
add_library(FilesTools FilesTools.h FilesTools.cpp)
add_library(AuxiliaryMethods AuxiliaryMethods.h AuxiliaryMethods.cpp)
add_subdirectory(graph)
add_subdirectory(generators)
add_subdirectory(circuits)
add_subdirectory(reliability)
target_link_libraries(prog TruthTable Reliability GraphVertex OrientedGraph Circuit AuxiliaryMethods FilesTools settings nlohmann_json::nlohmann_json)
add_subdirectory(tests)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(prog PUBLIC "-lpthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(prog "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_include_directories(settings PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(FilesTools PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(AuxiliaryMethods PUBLIC ${CMAKE_SOURCE_DIR})