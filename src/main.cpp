#include <iostream>
#include "svg-format.hpp"

int main() {
    std::cout << SVG::Begin(200, 200) << std::endl;
    std::cout << Rectangle(0, 0, 10, 10).WithFill({1, 1, 1}).Format()
              << std::endl;
    std::cout << Text(0, 0, "clock").Format() << std::endl;
    std::cout << LineSegment(0, 0, 10, 10).Format() << std::endl;
    std::cout << SVG::End() << std::endl;

    return 0;
}