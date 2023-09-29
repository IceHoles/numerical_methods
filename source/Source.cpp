﻿#include "rk.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>


void calculateTask1(double a, double b, double u, int Nmax, double epsilon, double h, std::ofstream& outputFile, std::ofstream& summary, bool conststep) {
    double x = a;
    double u_0 = u;
    double parameter = 0;
    double u1 = u;
    double u2 = u;
    double ue = u;
    double ele = 0;
    int C1 = 0;
    int C2 = 0;
    double maxele = 0;
    double maxh = h;
    double xmaxh = a;
    double minh = h;
    double xminh = a;
    int i = 0;
    double maxdev = 0;
    double xmaxdev = 0;
    if (conststep == 1) {
        for (; x <= b - 0.01 && i < Nmax; i++) {
            u1 = rk(parameter, x, u, h, f0);
            u2 = rkh(parameter, x, u, h, f0);
            ue = f0sol(x + h, u_0);

            ele = abs(u1 - u2) / 15;
            if (maxele < ele) { maxele = ele; }
            if (maxdev < abs(ue - u1)) { maxdev = abs(ue - u1); xmaxdev = x; }

            x += h;
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue - u1) << "\n";
        }
        summary << i << "," << b - x << "," << maxele << "," << maxdev << "," << xmaxdev << "\n";
    }
    else {
        for (; x <= b - 0.01 && i < Nmax; i++) {
            double lasth = h;
            int c1 = C1;
            int c2 = C2;
            u1 = rkv(parameter, x, u, h, ele, epsilon, C1, C2, f0);

            if (maxele < abs(ele)) { maxele = abs(ele); }
            if (maxh < h) { maxh = h; xmaxh = x; }
            if (minh > h) { minh = h; xminh = x; }
            if (c1 == (C1 - 1) && c2 == (C2 - 1)) {
                lasth = h / 2;
                u2 = rkh(parameter, x, u, lasth, f0); 
                ue = f0sol(x + lasth, u_0);
                x += lasth;
            } else if (lasth < h) {
                ue = f0sol(x + lasth, u_0);
                u2 = rkh(parameter, x, u, lasth, f0);
                x += lasth;
            } else  {
                ue = f0sol(x + h, u_0);
                u2 = rkh(parameter, x, u, h, f0); 
                x += h;
            }
            if (maxdev < abs(ue - u1)) { maxdev = abs(ue - u1); xmaxdev = x; }
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue - u1) << "\n";
            
            if (h < 0.0000001) break;
        }
        summary << i << "," << b - x << "," << maxele << "," << C1 << "," << C2 << "," << maxh << "," << xmaxh << "," << minh << "," << xminh << "," << maxdev << "," << xmaxdev << "\n";
    }
}

void calculateTask2(double a, double b, double u, int Nmax, double epsilon, double h, std::ofstream& outputFile, std::ofstream& summary, bool conststep) {
    double x = a;
    double parameter = 3;
    double u1 = u;
    double u2 = u;
    double ele = 0;
    int C1 = 0;
    int C2 = 0;
    double maxele = 0;
    double maxh = h;
    double xmaxh = a;
    double minh = h;
    double xminh = a;
    int i = 0;
    if (conststep == 1) {
        for (; x <= b - 0.01 && i < Nmax; i++) {
            u1 = rk(parameter, x, u, h, f1);
            u2 = rkh(parameter, x, u, h, f1);

            ele = abs(u1 - u2) / 15;
            if (maxele < ele) { maxele = ele; }

            x += h;
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
        }
        summary << i << "," << b - x << "," << maxele << "\n";
    }
    else {
        for (; x <= b - 0.01 && i < Nmax; i++) {
            double lasth = h;
            int c1 = C1;
            int c2 = C2;
            u1 = rkv(parameter, x, u, h, ele, epsilon, C1, C2, f1);
            
            if (maxele < ele) { maxele = ele; }
            if (maxh < h) { maxh = h; xmaxh = x; }
            if (minh > h) { minh = h; xminh = x; }
            if (c1 == C1 - 1 && c2 == C2 - 1) {
                lasth = h / 2;
                u2 = rkh(parameter, x, u, lasth, f1);
                x += lasth;
            }
            else if (lasth < h){ 
                u2 = rkh(parameter, x, u, lasth, f1);
                x += lasth;
            }
            else { 
                u2 = rkh(parameter, x, u, h, f1);
                x += h;
            }
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
            
            if (h < 0.0000001) break;
        }
        summary << i << "," << b - x << "," << maxele << "," << C1 << "," << C2 << "," << maxh << "," << xmaxh << "," << minh << "," << xminh << "\n";
    }
}

void calculateTask3(double parameter, double a, double b, double u, double du, int Nmax, double epsilon, double h, std::ofstream& outputFile, std::ofstream& summary, bool conststep) {
    double x = a;
    double ele = 0;
    int C1 = 0;
    int C2 = 0;
    double maxele = 0;
    double maxh = h;
    double xmaxh = a;
    double minh = h;
    double xminh = a;
    int i = 0;
    std::vector<double> uv = { u, du };
    std::vector<double> uv1;
    if (conststep == 1) {
        for (; x <= b - 0.01 && i < Nmax; i++) {
            uv1 = rk(parameter, x, uv, h, f2);
            std::vector<double> u1d = rkh(parameter, x, uv, h, f2);

            for (int i = 0; i < uv1.size(); i++) {
                ele = abs(uv1[i] - u1d[i]) / 15;
                if (maxele < ele) { maxele = ele; }
            }

            x += h;
            uv = uv1;

            outputFile << i + 1 << "," << x << "," << uv1[0] << "," << uv1[1] << "," << u1d[0] << "," << u1d[1] << "," << uv1[0] - u1d[0] << "," << uv1[1] - u1d[1] << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
        }
        summary << i << "," << b - x << "," << maxele << "\n";
    }
    else {
        for (; x <= b - 0.01 && i < Nmax; i++) {
            double lasth = h;  
            int c1 = C1;
            int c2 = C2;
            uv1 = rkv(parameter, x, uv, h, ele, epsilon, C1, C2, f2);
            std::vector<double> u1d(uv1.size());
            if (maxele < ele) { maxele = ele; }
            if (maxh < h) { maxh = h; xmaxh = x; }
            if (minh > h) { minh = h; xminh = x; }

            if (c1 == C1 - 1 && c2 == C2 - 1) {
                lasth = h / 2;
                u1d = rkh(parameter, x, uv, lasth, f2);
                x += lasth;
            }
            if (lasth < h) {
                u1d = rkh(parameter, x, uv, lasth, f2);
                x += lasth;
            }
            else {
                u1d = rkh(parameter, x, uv, h, f2);
                x += h;
            }
            uv = uv1;

            outputFile << i + 1 << "," << x << "," << uv1[0] << "," << uv1[1] << "," << u1d[0] << "," << u1d[1] << "," << uv1[0] - u1d[0] << "," << uv1[1] - u1d[1] << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
            
            if (h < 0.0000001) break;
        }
        summary << i << "," << b - x << "," << maxele << "," << C1 << "," << C2 << "," << maxh<< "," << xmaxh << "," << minh << "," << xminh << "\n";
    }
}

int main()
{
    double a, b, epsilon, h, u, du, parameter;
    int Nmax;
    int task;
    bool conststep;

    std::ifstream paramsFile("input_params.txt");
    if (!paramsFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    paramsFile >> a >> b >> u >> epsilon >> h >> Nmax >> task >> conststep >> du >> parameter;

    std::ofstream outputFile("results.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    std::ofstream summary("summary.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    if (task == 1) {
        calculateTask1(a, b, u, Nmax, epsilon, h, outputFile, summary, conststep);
    }
    else if (task == 2) {
        calculateTask2(a, b, u, Nmax, epsilon, h, outputFile, summary, conststep);
    }
    else if (task == 3) {
        calculateTask3(parameter, a, b, u, du, Nmax, epsilon, h, outputFile, summary, conststep);
    }
    else {
        std::cerr << "Invalid task number." << std::endl;
        return 1;
    }

    outputFile.close();
    return 0;
}