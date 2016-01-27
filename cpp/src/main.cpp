//#include "day1.h"
//#include "day2.h"
//#include "day3.h"
//#include "day4.h"
//#include "day5.h"
//#include "day6.h"
#include "day7.h"

#include <iostream>

int main(int argc, char** argv)
{
    using namespace aoc;
    try {
        //Day1::execute();
        //Day2::execute();
        //Day3::execute();
        //Day4::execute();
        //Day5::execute();
        //Day6::execute();
        Day7::execute();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
