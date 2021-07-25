#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <time.h>
using namespace std;
typedef vector<char> c1d;
typedef vector<c1d> c2d;
typedef vector<c2d> c3d;
typedef vector<c3d> c4d;
typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double> d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;
typedef vector<d3d> d4d;

class TS
{
public:
    i1d Run_result;
    i1d Run_iteration;

public:
    void run(int Run, int Iter, int T_size, const char *C, const char *W, const char *P)
    {
        d1d Capacity = read(C);
        d1d Weight = read(W);
        d1d Profit = read(P);

        int Dim = Weight.size();
        srand((unsigned)time(NULL));
        Run_set(Iter, Run);
        double START, END;

        START = clock();
        for (int i = 0; i < Run; i++)
        {
            int iteration = 0;
            Set(Dim);
            Initial(Weight, Capacity, Profit, T_size);
            while (iteration < Iter)
            {
                i1d New_H = Transition(Weight, Capacity, Profit, T_size);
                int New_Objective = Evaluation(New_H, Profit);
                Determination(New_H, New_Objective);
                Run_iteration[iteration] += Objective;
                iteration++;
            }
            Run_result[i] = Objective;
        }
        END = clock();
        Output(Run, Iter, Dim, START, END);
    }

private:
    i1d H;
    i2d T_table;
    double Objective;

private:
    d1d read(const char *A)
    { //讀檔
        d1d temp;
        fstream file;
        file.open(A, ios::in);
        if (!file)
        {
            cout << "open file error!" << endl;
            return temp;
        }
        else
        {
            double K;
            while (file >> K)
            {
                temp.push_back(K);
            }
            return temp;
        }
    }
    void Run_set(int iteration, int Run) //Set parameters for recording
    {
        Run_iteration.resize(iteration, 0);
        Run_result.resize(Run, 0);
    }
    inline int randomInt(int min, int max) //random number from min to max.
    {
        return (rand() % (max - min + 1) + min);
    }
    void Set(int Dim) //Set parameters for Tabu Search .
    {
        H.clear();
        H.swap(H);
        H.resize(Dim, 0);
        Objective = 0;
    }
    void Record(int T_size)
    {
        if (T_table.size() <= T_size)
            T_table.push_back(H);
        else
            T_table.pop_back();
    }
    void Initial(d1d Weight, d1d Capacity, d1d Profit, int T_size)
    {
        for (int i = 0; i < H.size(); i++)
        {
            H[i] = randomInt(0, 1);
        }
        if (!Check(H, Weight, Capacity))
            Initial(Weight, Capacity, Profit, T_size);
        Record(T_size);
        Objective = Evaluation(H, Profit);
    }
    bool Check(i1d arr, d1d Weight, d1d Capacity) //check the weight and check the tabu table
    {
        double W = 0.0;
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i] == 1)
                W += Weight[i];

            if (W > Capacity[0])
                return false;
        }
        for (int i = 0; i < T_table.size(); i++)
        {
            if (T_table[i] == arr)
                return false;
        }
        return true;
    }
    i1d NeighborSelection()
    {
        int Change_index = randomInt(0, H.size() - 1);
        i1d temp = H;
        if (temp[Change_index] == 0)
            temp[Change_index] = 1;
        else
            temp[Change_index] = 0;

        return temp;
    }
    i1d Transition(d1d Weight, d1d Capacity, d1d Profit, int T_size)
    {
        i1d temp = NeighborSelection();
        if (!Check(temp, Weight, Capacity))
        {
            temp = Transition(Weight, Capacity, Profit, T_size);
        }
        Record(T_size);
        return temp;
    }

    double Evaluation(i1d arr, d1d Profit)
    {
        double OBJ = 0;
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i] == 1)
                OBJ += Profit[i];
        }
        return OBJ;
    }
    void Determination(i1d NEW_H, int NEW_OBJ)
    {
        if (NEW_OBJ >= Objective)
        {
            H = NEW_H;
            Objective = NEW_OBJ;
        }
    }

    void Output(int run, int iteration, int dim, double START, double END)
    {
        int Run_AVG = 0;
        int Run_Best = 0;
        for (int i = 0; i < run; i++)
        {
            Run_AVG += Run_result[i];

            if (Run_result[i] > Run_Best)
                Run_Best = Run_result[i];
        }

        Run_AVG /= run;
        for (int i = 0; i < iteration; i++)
        {
            cout << i + 1 << ' ' << double(Run_iteration[i]) / run << endl; //using double to observe average convergence status.
        }
        cout << "# Run : " << run << endl;
        cout << "# Iteration : " << iteration << endl;
        cout << "# Dimension : " << dim << endl;
        cout << "# Best Objective : " << Run_Best << endl;
        cout << "# Average Objective : " << Run_AVG << endl;
        cout << "# Execution Time : " << (END - START) / CLOCKS_PER_SEC << "(s)" << endl;
    }
};
