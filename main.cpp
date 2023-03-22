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

void doCalcs(std::vector<double>& x, std::vector<double>& y, const double p_left, const double p_right, double step, const int& up_const, const int& loop_const, const int& max_down, const int thread_rank) {
    int up;
    int down;
    // std::cout << "PROGRESS " << thread_rank << std::endl;
    // if (thread_rank == 3) {
    //     std::cout << "p_right   " << p_right << std::endl;
    //     std::cout << "p_left   " << p_left << std::endl;
    //     std::cout << "step   " << step << std::endl;
    //     std::cout <<"p    ";
    // }
    // if (thread_rank == 0) {
    //     std::cout << "p_right   " << p_right << std::endl;
    //     std::cout << "p_left   " << p_left << std::endl;
    //     std::cout << "step   " << step << std::endl;
    //     std::cout <<"p    ";
    // }
    for (double p = p_right; p > p_left; p=p-step) { // 9% -12% -15% переход
        double sum = 0;
        // if (thread_rank == 3) {
        // std::cout << p << " ";
        // }
        // if (thread_rank == 0) {
        // std::cout << p << " ";
        // }
        for (int j=0; j<loop_const; j++) {
            // std::cout << "Completed %f%\r" << (j/loop_const)*100;
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
        x.push_back((double)(p));
        y.push_back((double)(sum/loop_const));

        std::cout << "PROGRESS " << thread_rank << " " << 100-round((double)((p-p_left)*100*100/(p_right-p_left)))/100 << "%" << std::endl;
        
        // std::cout << "PROGRESS " << 100-round((double)((p-p_left)*100*100/(p_right-p_left)))/100 << "%\r";
        // std::cout << 100-round((double)((p-p_left)*100*100/(p_right-p_left)))/100 << "%\n";
    }
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
}

void plotGraph(std::vector<double>& x, std::vector<double>& y) {
    std::cout << "Here is plotGraph"  << x.size() << y.size() << x.at(0) << " " << y.at(0) << std::endl;
    // for (double i: x) {
    //     std::cout << i << " ";
    // }
    // for (std::vector<double>::iterator it = y.begin(); it != y.end(); it++) {
    //     std::cout << *it << "";
    // }
    std::cout << *(y.begin()) << std::endl;
    std::cout << "CHECKPOINT" << std::endl;
    // std::cout << "" << std::endl;  // Why is it crahing?
    // for (auto j: y) {
    //     std::cout << j;
    // }
    // for (int i = 0; i< x.size(); i++) {
    //     std::cout << x.at(i) << " " << y.at(i) << std::endl;
    // }
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

// void y_plus(std::vector<double>& y, double* arr) {
//     // for (int i = 0; i<y.size(); i++) {
//     //     y.at(i) += arr[i];
//     // }
//     for (std::vector<double>::iterator it = y.begin(); it != y.end(); it++) {
//         *it += arr; 
//     }
// }

// void divide(std::vector<double>& y, int const n) {
//     // for (auto& v: y) {
//     //     v /= n;
//     // }
//     for (std::vector<double>::iterator it = y.begin(); it != y.end(); it++) {
//         *it /= n; 
//     }
// }

void concatinate(std::vector<double>& x, std::vector<double>& y, double* arr_recv, const int size) {
    for (int i = 0; i< size/2; i++) { 
        y.push_back(arr_recv[i]);
        x.push_back(arr_recv[(size/2)+i]);
    }
}

int main(int argc, char **argv) {
    if (argc != 7) {
        std::cerr << "Error: Not all parameters were passed" << std::endl;
        return -1;
    }

    // p = 0.2;
    // step = 0.001;
    // up_const = 50;
    // loop_const = 5;
    // max_down = 200;

    double p_left = std::stod(argv[1]);
    double p_right = std::stod(argv[2]);
    double const step = std::stod(argv[3]);
    int const up_const = std::stoi(argv[4]);
    int const loop_const = std::stoi(argv[5]);
    int const max_down = std::stoi(argv[6]);

    if (p_right <= p_left) {
        std::cerr << "Error: p_right less or equal to p_left" << std::endl;
        return -1;
    }

    int thread_size = 0;
    int thread_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &thread_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
    
    std::vector<double> x = {};
    std::vector<double> y = {};

    double b = (double)((p_right - p_left)/thread_size); // step of process
    p_left = p_left + b*thread_rank; // p_left of process
    p_right = p_left + b; // p_right of process
    std::cout << "pleft and pright " << thread_rank << " " << p_left << " " << p_right << std::endl;
    doCalcs(x, y, p_left, p_right, step, up_const, loop_const, max_down, thread_rank);
    std::cout << "SIZE IS " << x.size()+y.size() << std::endl;
    std::cout << "X.SIZE IS " << x.size() << std::endl;
    std::cout << "Y.SIZE IS " << y.size() << std::endl;
    if (thread_rank == 0) {
        // double arr_recv[x.size()+y.size()];
        // std::vector<double> arr_recv = {};

        // double* arr_recv = new double[(int)ceil((p_right - p_left)/(step*thread_size))];
        double* arr_recv = new double[x.size()+y.size()];

        // std::cout << "!!!!!" << (int)ceil((p_right - p_left)/(step*thread_size)) << std::endl;
        // std::cout << "!!!!!" << p_right - p_left << std::endl;
        // std::cout << "!!!!!" << step*thread_size << std::endl;
        // std::cout << "!!!" << ceil((p_right - p_left)/(step*thread_size)) << std::endl;
        // std::cout << "!!!!!" << x.size()+y.size() << std::endl;
        
        MPI_Status status_recv;
        // const int size = x.size()+y.size();
        int* size = new int[1];
        for (int i=0; i<thread_size-1; i++) {
            // std::cout << "RECV EROR" << std::endl;
            MPI_Recv(size, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, &status_recv);
            MPI_Recv(arr_recv, *size, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD, &status_recv);
            // y_plus(y, arr_recv);
            // std::cout << "AFTER RECV EROR" << std::endl;
            // std::cout << "" << std::endl;
            // std::cout << "" << std::endl;
            // std::cout << "PRINT ";
            // for (int i = 0; i<x.size()+y.size(); i++) {
            // std::cout << arr_recv[i] << " ";
            // }
            // std::cout << "" << std::endl;
            // std::cout << "" << std::endl;
            // std:: cout << "PRINT " << arr_recv[0];
            // std::cout << "AFTER RECV FOR" << std::endl;
            // std::cout << "AFTER RECV EROR" << std::endl;
            std::cout << "x.size()+y.size()    " << *size << std::endl;
            concatinate(x, y, arr_recv, *size);
            std::cout << "AFTER CONCAT EROR" << std::endl;
        }
        std::cout << "delete arr_recv ptr" << std::endl;
        arr_recv = nullptr;
        std::cout << "delete size ptr" << std::endl;
        size = nullptr;
        // delete arr_recv;
        // delete size;
        // divide(y, loop_const*thread_size);
        std::cout << "plotGraph(x, y);" << std::endl;
        std::cout << "x size is " << x.size() << std::endl;
        std::cout << "y size is " << y.size() << std::endl;
        for (double i: x) {
            std::cout << i << " ";
        }
        std::cout << "" << std::endl;  // Why is it crahing?
        for (double j: y) {
            std::cout << j << " ";
        }
        std::cout << "" << std::endl;
        std::cout << " BEFORE plotGraph(x, y);" << std::endl;
        plotGraph(x, y);

        std::cout << " AFTER plotGraph(x, y);" << std::endl;
    } else {
        // double arr_recv[x.size()+y.size()];
        std::vector<double> arr_recv(x.size()+y.size(),0);
        // std::cout << "BEFORE SEND: for EROR" << std::endl;
        for (int i = 0; i<y.size(); i++) {
            arr_recv[i] = y.at(i);
            arr_recv[y.size()+i] = x.at(i);
        }
        // std::cout << "thread_rank " << thread_rank << " ";
        // for (int i = 0; i<arr_recv.size(); i++) {
        //     std:: cout << " " << arr_recv.data()[i];
        // }
        // std::cout << " " << std::endl;
        // std::cout << "SEND EROR" << std::endl;
        int size = x.size()+y.size(); 
        MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(arr_recv.data(), x.size()+y.size(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        // std::cout << " AFTER SEND EROR" << std::endl;
    }
    MPI_Finalize();
    return 0; 
}

// in order to run program:
//      mpiexec -n 5 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe <p_left> <p_right> <step up_const> <loop_const> <max_down>
//      mpiexec -n 5 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe 0.1 0.2 0.001 50 5 200
