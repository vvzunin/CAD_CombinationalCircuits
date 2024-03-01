cmake_minimum_required(VERSION 3.10.2)
project(tests)
SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
  AuxiliaryMethodsTests.cpp 
  #SettingsTests.cpp 
  SimpleGeneratorsTests.cpp 
  #TruthTableTests.cpp 
  #OrientedGraphTests.cpp 
  #OrientedGraphSysTests.cpp 
  #FilesToolsTests.cpp 
  #GraphVertexTests.cpp
  )

target_link_libraries(${PROJECT_NAME} PUBLIC 
AuxiliaryMethods 
Circuit 
Reliability
SimpleGenerators 
TruthTable 
OrientedGraph 
GeneticParameters
MutationParameters 
RecombinationParameters 
SelectionParameters 
ParentsParameters 
GraphVertex 
FilesTools 
Settings 
OptimizationUtils 
ThreadPool 

nlohmann_json::nlohmann_json
gtest
gtest_main
)
