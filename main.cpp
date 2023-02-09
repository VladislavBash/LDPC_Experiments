#include <iostream>
// #include "sciplot/sciplot.hpp"
#include "library.h"

int main() {
    std::cout << "Hello, world!\n";
    int up;
    int down;
    double step = 0.05;
    for (double p = 0.5; p > 0; p-step) {
        up = 0;
        down = 0;
        while (up != 50) {
            if (decode(p)) {
                up++;
            }
            down++;
        }

    }
    return 0;
}
