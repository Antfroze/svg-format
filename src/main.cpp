#include "svg-format.hpp"

using namespace svg_fmt;

int main() {
    std::cout << SVG::Begin(200, 200) << std::endl;
    std::cout << Rectangle(0, 0, 10, 10).WithFill({1, 1, 1}) << std::endl;
    std::cout << Text(0, 0, "clock") << std::endl;
    std::cout << LineSegment(0, 0, 10, 10) << std::endl;
    std::cout << Triangle(0, 10, 10, 0, 0, 10).Open() << std::endl;
    std::cout << Circle(0, 0, 10) << std::endl;
    std::cout << SVG::End() << std::endl;

    return 0;
}