#include "ODE_Solver.hh"

std::array<double, 2> ODESolver::rk2solve(std::string functionexpr, std::map<char, double> variable_dict, char y0_var, double T) {
    InfixTools infix_tools;
    double k1, k2, tMid, yMid;

    std::string postfix_function = infix_tools.infix_to_postfix(functionexpr);
    
    double ti = 0;
    double yi = variable_dict[y0_var];

    // int numSteps = static_cast<int>(1.0 / stepSize + 0.5);
    while(yi<T){
        double prev_ti = ti;
        double prev_yi = yi;
        k1 = infix_tools.evalute_postfix_func(postfix_function, variable_dict);
        tMid = ti + (stepSize/2);
        yMid = yi + (stepSize/2)*k1;
        variable_dict[y0_var] = yMid;
        k2 = infix_tools.evalute_postfix_func(postfix_function, variable_dict);
        ti = ti + stepSize;
        yi = yi + stepSize*k2;

        if(yi<=0) break;
        double slope   = (y - prev_yi) / (t - prev_ti);
        if(std::abs(slope)<0.005) break;
        std::cout << ti << " | " << yi << std::endl;
    }

    std::cout << "Solution: " << ti << " & " << yi << std::endl; 
    return {ti, yi};
}
