cmake_minimum_required(VERSION 3.7)

project(basler LANGUAGES CXX)

file(GLOB SRCS *.cpp)

add_library(${PROJECT_NAME} SHARED ${SRCS})

execute_process(COMMAND $ENV{PYLON_ROOT}/bin/pylon-config --cflags
    OUTPUT_VARIABLE PYLON_CFLAGS)
string(REGEX REPLACE "[ \t\n]+$" "" PYLON_CFLAGS "${PYLON_CFLAGS}")
execute_process(COMMAND $ENV{PYLON_ROOT}/bin/pylon-config --libs-rpath
    OUTPUT_VARIABLE PYLON_LDFLAGS)
string(REGEX REPLACE "[ \t\n]+$" "" PYLON_LDFLAGS "${PYLON_LDFLAGS}")
execute_process(COMMAND $ENV{PYLON_ROOT}/bin/pylon-config --libs
    OUTPUT_VARIABLE PYLON_LDLIBS)
string(REGEX REPLACE "[ \t\n]+$" "" PYLON_LDLIBS "${PYLON_LDLIBS}")
    
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${MY_ROOT_DIR})

target_link_libraries(${PROJECT_NAME}
)

# -Wpsabi is a warning about possible compatibility problems because of an ABI change in GCC 7
# Using GCC 6.3 so no need to worry about this yet
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    COMPILE_FLAGS "${PYLON_CFLAGS} -Wno-unknown-pragmas -Wno-pedantic -Wno-psabi"
    LINK_FLAGS "${PYLON_LDFLAGS} ${PYLON_LDLIBS}"
)
