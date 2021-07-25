#include "normal.hpp"

int main(int argc, const char *argv[])
{
    int Run = atoi(argv[1]);
    int Iteration = atoi(argv[2]);
    double T_size = atoi(argv[3]);
    const char *Capacity = argv[4];
    const char *Weight = argv[5];
    const char *Profit = argv[6];
    TS ts;
    ts.run(Run, Iteration, T_size, Capacity, Weight, Profit);
}