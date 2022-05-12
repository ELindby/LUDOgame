cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project( Domain-Code )

file(GLOB SOURCES
    Domain-Code/.h
    Domain-Code/.hpp
    Domain-Code/*.cpp
)

# # Set outputfolder where program will be created
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

add_executable( main ${SOURCES} )