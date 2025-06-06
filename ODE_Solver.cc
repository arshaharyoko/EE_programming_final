#include "ODE_Solver.hh"

std::array<double, 2> ODESolver::rk2solve() {
    double k1, k2, tMid, yMid;
    double ti = t0;
    double yi = y0;
    int numSteps = static_cast<int>(1.0 / stepSize + 0.5);
    for (int stepCount = 0;  stepCount < numSteps;  ++stepCount) {
        k1 = func(ti, yi);
        tMid = ti + (stepSize/2);
        yMid = yi + (stepSize/2)*k1;
        k2 = func(tMid, yMid);
        ti = ti + stepSize;
        yi = yi + stepSize*k2;
    }
    return {ti, yi};
}

double ODESolver::func(double t, double y) {
    return t*t + y;
}