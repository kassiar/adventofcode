#pragma once

#ifndef AOC_DAY1_H
#define AOC_DAY1_H

#include "utils.h"

#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace aoc {

class Day1 {
public:
    static void execute()
    {
        const std::string& contents = read_file_into_string("../../input/day1.txt");
        int floor = 0;
        int basement_entrance_position = -1;
        for (auto it = contents.begin(), e = contents.end(); it != e; ++it)
        {
            switch (*it)
            {
                case '(':
                    floor++;
                    break;
                case ')':
                    floor--;
                    break;
                default:
                    throw std::runtime_error("input parse error");
            }
            if (floor == -1 && basement_entrance_position == -1) {
                basement_entrance_position = std::distance(contents.begin(), it) + 1;
            }
        }
        std::cout << "Final floor: " << floor << "\n"
                  << "First basement entrance: " << basement_entrance_position << std::endl;
    }
};

}



#endif // AOC_DAY1_H
