file(GLOB AOC_SOURCES *.cpp *.h)
add_executable(AOC ${AOC_SOURCES})
target_include_directories(AOC PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(AOC PRIVATE ${Boost_LIBRARIES})

