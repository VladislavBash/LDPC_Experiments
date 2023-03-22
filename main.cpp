#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include <sciplot/sciplot.hpp>
#include "library.h"
#include "mpi.h"
using namespace sciplot;

void doCalcs(std::vector<double>& x, std::vector<double>& y, const double p_left, const double p_right, double step, const int& up_const, const int& loop_const, const int& max_down, const int thread_rank) {
    int up;
    int down;
    for (double p = p_right; p > p_left; p=p-step) { // 9%-12%-15% transition
        double sum = 0;
        for (int j=0; j<loop_const; j++) {
            up = 0;
            down = 0;
            while (up != up_const) {
            if (decode(p)) {
                up++;
            }
            down++;
            if (down == max_down)
                break;
            }
        sum += ((double)up/down);
        }
        x.push_back((double)(p));
        y.push_back((double)(sum/loop_const));

        std::cout << "PROGRESS " << thread_rank << " " << 100-round((double)((p-p_left)*100*100/(p_right-p_left)))/100 << "%" << std::endl;
    }
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
}

void plotGraph(std::vector<double>& x, std::vector<double>& y, double const p_visible_left, double const p_visible_right) {
    Plot2D plot;
    plot.xlabel("x");
    plot.ylabel("y");
    plot.xrange(p_visible_left, p_visible_right);
    plot.yrange(0.0, 1.1);
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

void concatinate(std::vector<double>& x, std::vector<double>& y, double* arr_recv, const int size) {
    for (int i = 0; i< size/2; i++) { 
        y.push_back(arr_recv[i]);
        x.push_back(arr_recv[(size/2)+i]);
    }
}

int main(int argc, char **argv) {
    if (argc != 9) {
        std::cerr << "Error: Not all parameters were passed" << std::endl;
        return -1;
    }

    double const p_visible_left = std::stod(argv[1]);
    double const p_visible_right = std::stod(argv[2]);
    if (p_visible_left < 0) {
        std::cerr << "Error: p_visible_left less than zero" << std::endl;
        return -1;
    }
    if (p_visible_right <= p_visible_left) {
        std::cerr << "Error: p_visible_right is equal or less than p_visible_left" << std::endl;
        return -1;
    }
    double p_left = std::stod(argv[3]);
    double p_right = std::stod(argv[4]);
    double const step = std::stod(argv[5]);
    int const up_const = std::stoi(argv[6]);
    int const loop_const = std::stoi(argv[7]);
    int const max_down = std::stoi(argv[8]);

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
    doCalcs(x, y, p_left, p_right, step, up_const, loop_const, max_down, thread_rank);
    if (thread_rank == 0) {

        MPI_Status status_recv;
        
        int* size = new int[1];
        for (int i=0; i<thread_size-1; i++) {
            
            MPI_Recv(size, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, &status_recv);
            double* arr_recv = new double[*size];
            MPI_Recv(arr_recv, *size, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD, &status_recv);
            concatinate(x, y, arr_recv, *size);
        }
        x.emplace(x.begin(), 0);
        x.emplace(x.end(), 0.5);
        y.emplace(y.begin(), 1);
        y.emplace(y.end(), 0);
        plotGraph(x, y, p_visible_left, p_visible_right);
    } else {
        
        std::vector<double> arr_recv(x.size()+y.size(),0);
        
        for (int i = 0; i<y.size(); i++) {
            arr_recv[i] = y.at(i);
            arr_recv[y.size()+i] = x.at(i);
        }
        
        int size = x.size()+y.size(); 
        MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(arr_recv.data(), x.size()+y.size(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        
    }
    MPI_Finalize();
    return 0; 
}

// in order to run program:
//      mpiexec -n 5 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe <p_visible_left> <p_visible_right> <p_left> <p_right> <step> <up_const> <loop_const> <max_down>
//      mpiexec -n 5 C:\Users\79150\Documents\Programming\LDPC_Experiments\build\LDPC_experiment.exe 0 0.25 0.08 0.15 0.0001 50 50 200
