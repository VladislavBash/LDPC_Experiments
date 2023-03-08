#include <iostream>
#include <algorithm>
#include <sciplot/sciplot.hpp>
#include "library.h"
using namespace sciplot;

void doCalcs(std::vector<double>& x, std::vector<double>& y, double step, const int up_const) {
    int up;
    int down;
    int const max_down = 100;
    for (double p = 0.5; p > 0; p=p-step) { // 9% -12% -15% переход
        double sum = 0;
        for (int j=0; j<30; j++) {
            up = 0;
            down = 0;
            while (up != up_const) {
            if (decode(p)) { // TODO: delete 'true ||'
                up++;
            }
            down++; // TODO: max_down = 10000
            if (down == max_down)
                std::cout << "MAX_DOWN" << std::endl;
                break;
            }
        sum += ((double)up/down);
        }
        x.push_back(p);
        // std::cout << "PROGRESS " << x.size()*10 << "%" << std::endl;
        y.push_back(sum/30.0);
    }
}

void plotGraph(const std::vector<double>& x, const std::vector<double>& y) {
    Plot2D plot;
    plot.xlabel("x");
    plot.ylabel("y");
    plot.xrange(0.0, 1.0);
    plot.yrange(0.0, 2.0);
    plot.legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);
    plot.drawCurveWithPoints(x, y)
        .label("func")
        .lineWidth(0.3)
        .pointType(5)
        .pointSize(1);
    Figure fig = {{plot}};
    Canvas canvas = {{fig}};
    canvas.size(720, 720/2);
    canvas.title("LPDC_graph");
    canvas.show();
    canvas.save("plot.png");
}

int main() {
    std::vector<double> x = {};
    std::vector<double> y = {};
    doCalcs(x, y, 0.05, 50);
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
    plotGraph(x, y);
    return 0;
}
