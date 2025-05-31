#include "./Solver_Client.hh"

int main() {
    SolverClient ode_solver;

    ode_solver.define_socket();
    ode_solver.connect_socket();
    ode_solver.request();

    ode_solver.destroy();
    return 0;
}