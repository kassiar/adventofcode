add_subdirectory(Utils)

set(AOC_SOURCES main.cpp
				day1.h
				day2.h
				day3.h
				day4.h
				day5.h
				day6.h
				day7.h
				day8.h
				)
add_executable(AOC ${AOC_SOURCES})
target_include_directories(AOC PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(AOC PRIVATE ${Boost_LIBRARIES} PRIVATE Utils)
