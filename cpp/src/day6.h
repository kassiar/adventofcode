#pragma once

#ifndef AOC_DAY6_H
#define AOC_DAY6_H

#include <iostream>
#include <regex>
#include <algorithm>
#include <numeric>

#include "utils.h"

namespace aoc {

class Day6 {
public:
    static void execute()
    {
        const std::string input = read_file_into_string("../../../input/day6.txt");
        std::istringstream input_stream(input);
        Grid grid1(1'000'000, 0);
        Grid grid2(1'000'000, 0);
        for (std::string line; std::getline(input_stream, line); )
        {
            Command command = parse_command(line);
            apply_command(command, grid1);
            apply_command_2(command, grid2);
        }        
        std::cout << "Lights turned on: " << std::count_if(grid1.begin(), grid1.end(), [](int val) { return val != 0; }) << std::endl;
        std::cout << "Total lights brightness: " << std::accumulate(grid2.begin(), grid2.end(), 0) << std::endl;
    }
private:
    using Grid = std::vector<int>;

    enum class CommandType {
        TURN_OFF,
        TURN_ON,
        TOGGLE
    };
    using Coords = std::pair<int, int>;

    struct Command {
        CommandType type;
        Coords begin;
        Coords end;
    };

    static Command parse_command(const std::string& str)
    {
        static std::regex command_regex("(turn on|turn off|toggle) ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)");
        std::smatch match;
        if (std::regex_match(str, match, command_regex))
        {
            Command command;
            const std::string& command_str = std::string(match[1].first, match[1].second);
            if (command_str.compare("turn on") == 0) {
                command.type = CommandType::TURN_ON;
            }
            else if (command_str.compare("turn off") == 0) {
                command.type = CommandType::TURN_OFF;
            }
            else if (command_str.compare("toggle") == 0) {
                command.type = CommandType::TOGGLE;
            }
            else {
                throw std::runtime_error(std::string("Invalid command type: ") + command_str);
            }
            command.begin.first = std::stoi(std::string(match[2].first, match[2].second));
            command.begin.second = std::stoi(std::string(match[3].first, match[3].second));
            command.end.first = std::stoi(std::string(match[4].first, match[4].second));
            command.end.second = std::stoi(std::string(match[5].first, match[5].second));

            return command;
        }
        else
        {
            throw std::runtime_error(std::string("Input parse error on line ") + str);
        }
    }

    static void apply_command(Command cmd, Grid& grid)
    {
        switch (cmd.type) {
            case CommandType::TURN_ON:
                for (int row = cmd.begin.first; row < cmd.end.first + 1; row++) {
                    for (int col = cmd.begin.second; col < cmd.end.second + 1; col++) {
                        grid[1000 * row + col] = 1;
                    }
                }
                break;
            case CommandType::TURN_OFF:
                for (int row = cmd.begin.first; row < cmd.end.first + 1; row++) {
                    for (int col = cmd.begin.second; col < cmd.end.second + 1; col++) {
                        grid[1000 * row + col] = 0;
                    }
                }
                break;
            case CommandType::TOGGLE:
                for (int row = cmd.begin.first; row < cmd.end.first + 1; row++) {
                    for (int col = cmd.begin.second; col < cmd.end.second + 1; col++) {
                        grid[1000 * row + col] = 1 - grid[1000 * row + col];
                    }
                }
                break;
            default:
                throw std::runtime_error("Invalid command type");
        }
    }

    static void apply_command_2(Command cmd, Grid& grid)
    {
        switch (cmd.type) {
            case CommandType::TURN_ON:
                for (int row = cmd.begin.first; row < cmd.end.first + 1; row++) {
                    for (int col = cmd.begin.second; col < cmd.end.second + 1; col++) {
                        grid[1000 * row + col]++;
                    }
                }
                break;
            case CommandType::TURN_OFF:
                for (int row = cmd.begin.first; row < cmd.end.first + 1; row++) {
                    for (int col = cmd.begin.second; col < cmd.end.second + 1; col++) {
                        grid[1000 * row + col] = std::max(grid[1000 * row + col] - 1, 0);
                    }
                }
                break;
            case CommandType::TOGGLE:
                for (int row = cmd.begin.first; row < cmd.end.first + 1; row++) {
                    for (int col = cmd.begin.second; col < cmd.end.second + 1; col++) {
                        grid[1000 * row + col] += 2;
                    }
                }
                break;
            default:
                throw std::runtime_error("Invalid command type");
        }
    }

};

} //namespace

#endif // AOC_DAY6_H
