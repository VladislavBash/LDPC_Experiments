#include <iostream>
#include <algorithm>
#include <sciplot/sciplot.hpp>
#include "library.h"
using namespace sciplot;

void doCalcs(std::vector<double>& x, std::vector<double>& y, double step, const int up_const) {
    int up;
    int down;
    for (double p = 0.5; p > 0; p=p-step) {
        up = 0;
        down = 0;
        while (up != up_const) {
            if (true || decode(p)) { // TODO: delete 'true ||'
                up++;
            }
            down++;
        }
        x.push_back(p);
        y.push_back((double)up/down);
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
    doCalcs(x, y, 0.05, 5);
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
    plotGraph(x, y);
    return 0;
}
