#pragma once

#ifndef AOC_DAY8_H
#define AOC_DAY8_H

#include "utils.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>

namespace aoc {

class Day8 {
public:
    static void execute()
    {
        const std::string& contents = read_file_into_string("../../../input/day8.txt");
        std::istringstream input(contents);
        size_t mem_chars_p1 = 0;
        size_t code_chars_p1 = 0;
        size_t mem_chars_p2 = 0;
        size_t code_chars_p2 = 0;
        for (std::string line; std::getline(input, line); )
        {
            const part1_visitor& r = std::for_each(line.begin(), line.end(), part1_visitor());
            mem_chars_p1 += r.mem_chars;
            code_chars_p1 += r.code_chars;

            std::for_each(line.begin(), line.end(),
                          [&mem_chars_p2, &code_chars_p2](char c) {
                              mem_chars_p2++;
                              code_chars_p2 += (c == '\\' || c == '\"') ? 2 : 1;
                          });
            // 2 quotes are added to each processed string
            code_chars_p2 += 2;
        }
        std::cout << "part1: code_chars - mem_chars: " << code_chars_p1 - mem_chars_p1 << std::endl;
        std::cout << "part2: code_chars - mem_chars: " << code_chars_p2 - mem_chars_p2 << std::endl;

    }
private:
    struct part1_visitor
    {
        part1_visitor()
            : state(State::Normal),
              code_chars(0),
              mem_chars(0)
        {
            // empty
        }
        void operator()(char c)
        {
            code_chars++;
            switch (c)
            {
                case '\\':
                    if (state == State::ParsedEscape) {
                        state = State::Normal;
                        mem_chars++;
                    }
                    else {
                        state = State::ParsedEscape;
                    }
                    break;
                case '\"':
                    if (state == State::ParsedEscape) {
                        mem_chars++;
                        state = State::Normal;
                    }
                    break;
                case 'x':
                    if (state != State::ParsedEscape) {
                        mem_chars++;
                    }
                    else {
                        state = State::WaitingHex1;
                    }
                    break;
                default:
                    if (state == State::WaitingHex1) {
                        state = State::WaitingHex2;
                    }
                    else if (state == State::WaitingHex2) {
                        mem_chars++;
                        state = State::Normal;
                    }
                    else {
                        mem_chars++;
                    }
                    break;
            }
        }
        
        enum class State {
            Normal, ParsedEscape, WaitingHex1, WaitingHex2
        };

        size_t code_chars;
        size_t mem_chars;
        State state;
    };
};

}



#endif // AOC_DAY8_H
