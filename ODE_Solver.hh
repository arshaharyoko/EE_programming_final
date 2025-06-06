#ifndef H_ODE_SOLVER
#define H_ODE_SOLVER
#include <array>

class ODESolver {
    private:
        double t0 = 0;
        double y0 = 1;
        double stepSize = 0.05;
        double t, y;

    public:
        std::array<double, 2> rk2solve();
        double func(double t, double y);
};

#endif