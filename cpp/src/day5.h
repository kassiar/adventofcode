#pragma once

#ifndef AOC_DAY5_H
#define AOC_DAY5_H

#include <sstream>
#include <iostream>
#include <set>
#include <regex>

#include "utils.h"


namespace aoc {

class Day5 {
public:
    static void execute()
    {
        const std::string input = read_file_into_string("../../../input/day5.txt");
        std::istringstream input_stream(input);

        const std::regex naughty_combo_regex("ab|cd|pq|xy");
        const std::regex three_vowels_regex("[aeiou].*[aeiou].*[aeiou]");
        const std::regex letter_pair_regex("([[:alpha:]])\\1");
        const std::regex repeating_pairs_regex("([[:alpha:]]{2}).*\\1");
        const std::regex triplet_regex("([[:alpha:]]).\\1");

        size_t nice_strings_count = 0;
        size_t nice_strings_count_2 = 0;
        for (std::string line; std::getline(input_stream, line); )
        {
            if (!std::regex_search(line, naughty_combo_regex) &&
                std::regex_search(line, three_vowels_regex) &&
                std::regex_search(line, letter_pair_regex))
            {
                nice_strings_count++;
            }
            if (std::regex_search(line, repeating_pairs_regex) &&
                std::regex_search(line, triplet_regex))
            {
                nice_strings_count_2++;
            }
        }
        std::cout << "Nice strings number: " << nice_strings_count << "\n"
                  << "Nice strings number 2: " << nice_strings_count_2 << std::endl;
    }
};

} //namespace

#endif // AOC_DAY5_H
