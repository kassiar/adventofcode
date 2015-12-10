#pragma once

#ifndef AOC_DAY2_H
#define AOC_DAY2_H

#include "utils.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace aoc {


static int required_paper_area(int length, int width, int height)
{
    const int area1 = width*height;
    const int area2 = height*length;
    const int area3 = length*width;
    return 2 * (area1 + area2 + area3) + std::min(std::min(area1, area2), area3);
}

static int required_ribbon_length(int length, int width, int height)
{
    const int hperim1 = width + length;
    const int hperim2 = length + height;
    const int hperim3 = height + width;
    return 2 * std::min(std::min(hperim1, hperim2), hperim3) + width*length*height;
}

template <typename T>
T from_string(const std::string& s)
{
    std::istringstream sstream(s);
    T t;
    sstream >> t;
    if (sstream.fail() || sstream.bad()) throw std::runtime_error("parse error");
    return t;
}

class Day2 {
public:
    static void execute()
    {
        std::ifstream inputfile("../../input/day2.txt");
        if (!inputfile.is_open()) throw std::runtime_error("open file error");
        int total_wrapper_size = 0;
        int total_ribbon_size = 0;
        for (std::string line; std::getline(inputfile, line); )
        {
            std::istringstream localstream(line);
            int x, y, z;
            std::string value;
            std::getline(localstream, value, 'x');
            x = from_string<int>(value);
            std::getline(localstream, value, 'x');
            y = from_string<int>(value);
            std::getline(localstream, value, 'x');
            z = from_string<int>(value);
            total_wrapper_size += required_paper_area(x, y, z);
            total_ribbon_size += required_ribbon_length(x, y, z);
        }
        std::cout << "Wrapper: " << total_wrapper_size << ", ribbon: " << total_ribbon_size << std::endl;
        return;
    }
};

} // namespace aoc

#endif // AOC_DAY2_H
