#pragma once

#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <fstream>
#include <string>
#include <sstream>

namespace aoc {

std::string read_file_into_string(const std::string& filename)
{
    std::ifstream filestream(filename);
    if (!filestream.is_open()) { throw std::runtime_error("open file error"); }
    std::ostringstream content_stream;
    content_stream << filestream.rdbuf();
    return content_stream.str();
}

}

#endif // AOC_UTILS_H
