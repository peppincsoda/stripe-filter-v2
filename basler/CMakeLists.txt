cmake_minimum_required(VERSION 3.7)

project(baslercam LANGUAGES CXX)

file(GLOB SRCS *.cpp)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${MY_ROOT_DIR})

target_link_libraries(${PROJECT_NAME}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
