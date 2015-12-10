#pragma once

#ifndef AOC_DAY3_H
#define AOC_DAY3_H

#include "utils.h"

#include <string>
#include <map>
#include <algorithm>
#include <iostream>

namespace aoc {

class SantaTracker {
public:

    using House = std::pair<int, int>;
    using PresentsCount = int;
    using PresentsMap = std::map<House, PresentsCount>;

    SantaTracker(bool roboSantaEnabled_) {
        positions[0] = positions[1] = House(0, 0);
        currentPerson = 0;
        houseMap[House(0, 0)] = 2;
        roboSantaEnabled = roboSantaEnabled_;
    }
    void operator()(char c) {
        House& position = positions[currentPerson];
        switch (c) {
            case '<':
                position.first--;
                break;
            case '^':
                position.second++;
                break;
            case '>':
                position.first++;
                break;
            case 'v':
                position.second--;
                break;
            default:
                throw std::logic_error("invalid input character");
        }
        houseMap[position] += 1;
        if (roboSantaEnabled) currentPerson = 1 - currentPerson;
    }

    size_t visitedHousesNumber() const {
        return houseMap.size();
    }
private:
    PresentsMap houseMap;
    House positions[2];
    int currentPerson;
    bool roboSantaEnabled;
};

class Day3 {
public:
    static void execute()
    {
        const std::string contents = read_file_into_string("../../input/day3.txt");
        const SantaTracker& santaOnlyTracker = std::for_each(contents.begin(), contents.end(), SantaTracker(false));
        const SantaTracker& robosantaTracker = std::for_each(contents.begin(), contents.end(), SantaTracker(true));
        std::cout << "Solo Santa result: " << santaOnlyTracker.visitedHousesNumber() << "\n"
                  << "Santa couple result: " << robosantaTracker.visitedHousesNumber() << std::endl;
    }
};

} // namespace aoc

#endif // AOC_DAY3_H
