#include <iostream>
#include <algorithm>
#include <sciplot/sciplot.hpp>
#include "library.h"
#include "mpi.h"
using namespace sciplot;

void doCalcs(std::vector<double>& x, std::vector<double>& y, double step, const int up_const, const int loop_const) {
    int up;
    int down;
    int const max_down = 200;
    for (double p = 0.2; p > 0; p=p-step) { // 9% -12% -15% переход
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
        .pointType(5)
        .pointSize(1);
    Figure fig = {{plot}};
    Canvas canvas = {{fig}};
    canvas.size(720, 720/2);
    canvas.title("LPDC_graph");
    canvas.show();
    canvas.save("plot.png");
    std::cin.ignore();
}

void y_plus(std::vector<double>& y, double* arr) {
    for (int i = 0; i<y.size(); i++) {
        y.at(i) += arr[i];
    }
}

void divide(std::vector<double>& y, int const n) {
    for (auto& v: y) {
        v /= n;
    }
}

int main(int argc, char **argv) {
    // std::vector<double> x = {};
    // std::vector<double> y = {};
    // doCalcs(x, y, 0.05, 50);
    // std::reverse(x.begin(), x.end());
    // std::reverse(y.begin(), y.end());
    // plotGraph(x, y);
    int thread_size = 0;
    int thread_rank = 0;
    // std::cout << "Before parallel sect" << std::endl;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &thread_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
    // std::cout << "Parallel sect" << std::endl;
    // if (thread_rank == 0) {
    //     std::cout << "Thread_size " << thread_size << std::endl;
    //     std::cout << "Thread_rank_main " << thread_rank << std::endl;
    // } else {
    //     std::cout << "Thread_rank " << thread_rank << std::endl;
    // }

    // if (thread_rank == 0) {
    //     int num_recv = 0;
    //     // for (int i=0; i<thread_size; i++) {
    //     //     num_recv[i] = 0;
    //     // }
    //     int res_sum = 0;
    //     MPI_Status status_recv;
    //     for (int i=0; i<thread_size-1; i++) {
    //         MPI_Recv(&num_recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status_recv);
    //         res_sum += num_recv;
    //     }
    //     std::cout << res_sum << std::endl;
    // } else {
    //     int num_send = thread_rank; // result of doCalcs
    //     MPI_Send(&num_send, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // }

    std::vector<double> x = {};
    std::vector<double> y = {};
    int const loop_const = 5;
    // std::cout << "Before func\r";
    // std::cout << std::endl;
    doCalcs(x, y, 0.002, 50, loop_const);
    // std::cout << "After func\r";
    // std::cout << std::endl;
    if (thread_rank == 0) {
        // int num_recv = 0;
        // for (int i=0; i<thread
        // int res_sum = 0;
        // std::vector<std::vector<double>> arr_recv = {};
        double arr_recv[x.size()+y.size()];
        MPI_Status status_recv;

        for (int i=0; i<thread_size-1; i++) {
            MPI_Recv(&arr_recv, x.size()+y.size(), MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status_recv);
            // if (x != arr_revc.at(0)) { throw error }
            y_plus(y, arr_recv);//plus y (arr_recv.at(1))
        }

        divide(y, loop_const*thread_size); // divide loop_const*thread_size

        plotGraph(x, y);
    } else {
        // std::vector<std::vector<double>> arr_recv = {x, y};
        double arr_recv[x.size()+y.size()];
        for (int i = 0; i<x.size(); i++) {
            arr_recv[i] = y.at(i);
            arr_recv[50+i] = x.at(i);
        }
        MPI_Send(&arr_recv, x.size()+y.size(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0; 
}

// in order to run program:
//      mpiexec -np 4 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe
