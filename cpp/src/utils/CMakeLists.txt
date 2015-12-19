add_library(Utils STATIC utils.h md5.h md5.cpp)
target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
