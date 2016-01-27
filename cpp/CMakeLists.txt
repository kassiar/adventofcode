cmake_minimum_required(VERSION 3.0)

project(AdventOfCode)

# find Boost libraries
add_definitions(-DBOOST_ALL_NO_LIB)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.57 COMPONENTS system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(src)
