#ifndef H_ODE_SOLVER
#define H_ODE_SOLVER
#include "Infix_Tools.hh"
#include <array>

class ODESolver {
    private:
        double t0 = 0;
        double y0 = 1;
        double stepSize = 0.05;
        double t, y;

    public:
        std::array<double, 2> rk2solve(std::string functionexpr, std::map<char, double> variable_dict, char y0_var, double T);
};

#endif