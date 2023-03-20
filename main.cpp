#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include <sciplot/sciplot.hpp>
#include "library.h"
#include "mpi.h"
using namespace sciplot;

// max_down = 200;
// p = 0.2;
// step = 0.001;
// up_const = 50;
// loop_const = 5;

void doCalcs(std::vector<double>& x, std::vector<double>& y, double step, const int& up_const, const int& loop_const, const int& max_down, double p) {
    int up;
    int down;
    for (p; p > 0; p=p-step) { // 9% -12% -15% переход
        double sum = 0;
        for (int j=0; j<loop_const; j++) {
            // printf("Completed %f%\r",(j/loop_const)*100);
            up = 0;
            down = 0;
            while (up != up_const) {
            if (decode(p)) {
                up++;
            }
            down++;
            if (down == max_down)
                std::cout << "MAX_DOWN" << std::endl;
                break;
            }
        sum += ((double)up/down);
        }
        x.push_back(p);
        // std::cout << "PROGRESS " << x.size()*10 << "%" << std::endl;
        y.push_back(sum);
    }
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
}

void plotGraph(const std::vector<double>& x, const std::vector<double>& y) {
    Plot2D plot;
    plot.xlabel("x");
    plot.ylabel("y");
    plot.xrange(0.0, 0.25);
    plot.yrange(0.0, 1.25);
    plot.legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);
    plot.drawCurveWithPoints(x, y)
        .label("func")
        .lineWidth(0.3)
        .pointType(0)
        .pointSize(1);
    Figure fig = {{plot}};
    Canvas canvas = {{fig}};
    canvas.size(720, 720/2);
    canvas.title("LPDC_graph");
    canvas.show();
    canvas.save("plot.png");
    canvas.saveplotdata();
    std::cin.ignore();
}

void y_plus(std::vector<double>& y, double* arr) {
    // for (int i = 0; i<y.size(); i++) {
    //     y.at(i) += arr[i];
    // }
    for (std::vector<double>::iterator it = y.begin(); it != y.end(); it++) {
        *it += arr; 
    }
}

void divide(std::vector<double>& y, int const n) {
    // for (auto& v: y) {
    //     v /= n;
    // }
    for (std::vector<double>::iterator it = y.begin(); it != y.end(); it++) {
        *it /= n; 
    }
}

int main(int argc, char **argv) {
    if (argc != 5) {
        std::cerr << "Error: Not all parameters were passed" << std::endl;
        return -1;
    }

    // p = 0.2;
    // step = 0.001;
    // up_const = 50;
    // loop_const = 5;
    // max_down = 200;

    double const p = std::stod(argv[0]);
    double const step = std::stod(argv[1]);
    int const up_const = std::stoi(argv[2]);
    int const loop_const = std::stoi(argv[3]);
    int const max_down = std::stoi(argv[4]);

    int thread_size = 0;
    int thread_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &thread_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
    
    std::vector<double> x = {};
    std::vector<double> y = {};

    doCalcs(x, y, step, up_const, loop_const, max_down, p);

    if (thread_rank == 0) {
        // double arr_recv[x.size()+y.size()];
        // std::vector<double> arr_recv = {};
        double* arr_recv = new double[ceil(p/step)];
        MPI_Status status_recv;

        for (int i=0; i<thread_size-1; i++) {
            MPI_Recv(&arr_recv, x.size()+y.size(), MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status_recv);
            y_plus(y, arr_recv);
        }
        delete arr_recv;
        divide(y, loop_const*thread_size);

        plotGraph(x, y);
    } else {
        // double arr_recv[x.size()+y.size()];
        std::vector<double> arr_recv = {};

        for (int i = 0; i<y.size(); i++) {
            arr_recv[i] = y.at(i);
            arr_recv[y.size()+i] = x.at(i);
        }

        MPI_Send(arr_recv.data(), x.size()+y.size(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0; 
}

// in order to run program:
//      mpiexec -n 5 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe <p> <step up_const> <loop_const> <max_down>
//      mpiexec -n 5 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe 0.2 0.001 50 5 200
