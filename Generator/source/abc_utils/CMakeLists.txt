# Create executable
project(abc_utils)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

set (SOURCES
  abc_utils/AbcUtil.cpp
)

add_library(AbcUtil AbcUtil.cpp AbcUtil.h)
