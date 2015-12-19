#pragma once

#ifndef AOC_DAY4_H
#define AOC_DAY4_H

#include <iostream>
#include <string>
#include <future>
#include <algorithm>
#include <iterator>

#include "md5.h"

namespace aoc {

class Day4 {
public:
    static void execute()
    {
        const std::string input = "bgvyzdsv";
        const std::string pattern1 = "00000";
        const std::string pattern2 = "000000";
        std::cout << "Solving for " << pattern1 << std::endl;
        parallel_calc(input, pattern1);
        serial_calc(input, pattern1);
        std::cout << "Solving for " << pattern2 << std::endl;
        parallel_calc(input, pattern2);
        serial_calc(input, pattern2);
    }

private:
    static bool check_digest(const char* pattern, const char* digest, size_t len)
    {
        return std::strncmp(digest, pattern, len) == 0;
    }

    static void serial_calc(const std::string& input, const std::string& pattern)
    {
        long i = 0;
        const char* pattern_c = pattern.c_str();
        const size_t pattern_len = pattern.length();
        while (true)
        {
            if (check_digest(pattern_c, MD5(input + std::to_string(i)).hexdigest().c_str(), pattern_len)) {
                std::cout << "Found answer: " << i << std::endl;
                break;
            }
            i++;
        }
    }

    static void parallel_calc(const std::string& input, const std::string& pattern)
    {
        const long STEP = 500'000;
        const size_t THREADS_NUM = 4;
        const char* pattern_c = pattern.c_str();
        const size_t pattern_len = pattern.length();
        auto lambda = [=](long step_number) -> long
        {
            for (long val = step_number * STEP; val < (step_number + 1)*STEP; val++)
            {
                if (check_digest(pattern_c, MD5(input + std::to_string(val)).hexdigest().c_str(), pattern_len)) {
                    return val;
                }
            }
            return -1;
        };
        long i = 0;
        while (true)
        {
            std::cout << "Checking range: (" << i*STEP << ", " << (i + 4)*STEP << ")" << std::endl;
            std::vector<std::future<long>> futures;
            for (int k = 0; k < THREADS_NUM; k++) {
                futures.push_back(std::async(std::launch::async, lambda, i + k));
            }
            std::vector<long> results;
            for (int k = 0; k < THREADS_NUM; k++) {
                results.push_back(futures[k].get());
            }
            std::vector<long> non_default_results;
            std::copy_if(results.begin(), results.end(), std::back_inserter(non_default_results), [](long val) { return val != -1; });
            if (!non_default_results.empty()) {
                std::cout << "Found the answer: " << *std::min_element(non_default_results.begin(), non_default_results.end()) << std::endl;
                break;
            }
            i += THREADS_NUM;
        }
    }
};

} // namespace

#endif // AOC_DAY4_H
