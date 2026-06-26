#include "gtest_lite.h"
#include <iostream>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  HX Unit Tests - Binary Tree Traversal " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    int result = testing::TestRegistry::Instance().RunAll();

    std::cout << std::endl;
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " test(s) failed!" << std::endl;
    }

    return result;
}
