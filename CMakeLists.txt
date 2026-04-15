cmake_minimum_required(VERSION 3.28.3)
project(dsl)

# set(CMAKE_CXX_COMPILER g++-14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

option(BUILD_TESTS "GTest turned on")

set(SOURCES
  src/log.cpp
  
  
  src/pi/node/portable_node/sum_node.cpp
  src/pi/node/portable_node/end_node.cpp
  src/pi/node/portable_node/portable_node.cpp
  src/pi/node/portable_node/mul_node.cpp
  src/pi/node/portable_node/unit_delay_node.cpp

  src/pi/node/pi_node.cpp
  src/pi/node/start_node.cpp
  
  src/pi/parser/xml2pi-parser.cpp

  src/pi/generator/pi-c-generator.cpp
  
  src/pi/pi-transformer.cpp
  src/pi/pi-controller.cpp
)

if(BUILD_TESTS)
  set(SOURCES ${SOURCES} test/test.cpp)

  add_subdirectory(extern/googletest)
else()
  set(SOURCES ${SOURCES} src/main.cpp)
endif()

add_subdirectory(extern/pugixml)
add_subdirectory(extern/fmt)

add_executable(${PROJECT_NAME} ${SOURCES})

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak")

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE pugixml::pugixml fmt::fmt)
if(BUILD_TESTS)
  target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)
endif()
