cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project information
project(example
  LANGUAGES CXX
)

# Guard from building the project in-source.
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Dependencies
include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME lapwing
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)

# Create the standalone executable
file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES 
  CXX_STANDARD 20
  OUTPUT_NAME "${PROJECT_NAME}"
)

target_link_libraries(${PROJECT_NAME} lapwing)
