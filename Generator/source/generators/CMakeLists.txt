project(generator)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(Genetic)

add_library(TruthTable SHARED TruthTable.cpp TruthTable.h)
add_library(SimpleGenerators SHARED SimpleGenerators.cpp SimpleGenerators.h GenerationParameters.cpp GenerationParameters.h)
# add_library(Parser SHARED Parser.cpp Parser.h)

target_include_directories(TruthTable PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(SimpleGenerators PUBLIC ${CMAKE_SOURCE_DIR})
# target_include_directories(Parser PUBLIC ${CMAKE_SOURCE_DIR})
