cmake_minimum_required(VERSION 3.7)

project(sfv2)

enable_testing()

set(MY_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Enable profiling
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# The basler library (which is directly linked to the Basler SDK libs) is not supported on MINGW32
if(NOT MINGW)
    add_subdirectory(basler)
endif()
add_subdirectory(filterapp)
