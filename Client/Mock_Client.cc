#include "Solver_Client.hh"
#include "json_parser.hh"
#include <iostream>
#include <string>
#include <map>

using namespace std;

double mapLogisticGrowthRate(int rating) {
    double r;
    switch(rating) {
        case 1: r = 0.02; break; // Very Slow
        case 2: r = 0.05; break; // Slow
        case 3: r = 0.10; break; // Medium
        case 4: r = 0.20; break; // Fast
        case 5: r = 0.30; break; // Very Fast
        default:
            cout << "Invalid input. Using default (0.10).\n";
            r = 0.10;
    }
    return r;
}

// Maps a 1-5 rating to affection points
int mapAffection(int rating) { // A Coefficient
    switch (rating) {
        case 1: return 10; // Very Low
        case 2: return 25; // Low
        case 3: return 50; // Neutral
        case 4: return 75; // High
        case 5: return 90; // Very High
        default: return 0; // Invalid
    }
}

int main() {
    SolverClient ode_solver;
    JSONDataObject formulas;
    std::map<char, double> variable_dict;
    
    formulas.parseJSON(formulas.openFile("./formulas.json"));

    int choice = 0;
    while (true) {
        ode_solver.define_socket();
        ode_solver.connect_socket();
        variable_dict = {}; // Reinitialize/clear variable_dict
        cout << "\n--- Love Calculator ---" << endl;
        cout << "1. Linear Partner" << endl;
        cout << "2. Saturating Partner" << endl;
        cout << "3. Avoidant Partner" << endl;
        cout << "4. Exit" << endl;
        cout << "Choose your partner type (1-4): ";
        cin >> choice;

        if(choice == 4) break;

        int rating = 0;
        // Ask current affection level and map to points
        cout << "\nRate current affection level:" << endl;
        cout << "1. Very Low\n2. Low\n3. Neutral\n4. High\n5. Very High" << endl;
        cout << "Your choice (1-5): ";
        cin >> rating;
        int affectionPoints = mapAffection(rating);
        cout << "Current affection points: " << affectionPoints << "/100" << endl;

        int response;
        switch (choice) {
            case 1: {
                // Linear
                cout << "\n[Linear Model] Calculating time to reach 100 affection from "
                     << affectionPoints << "..." << endl;
                // Placeholder for actual computation using affectionPoints
                int idx = formulas.get_label_idx("Linear");
                variable_dict.insert({'k', 0.2});
                ode_solver.request(formulas.get_function(idx), variable_dict, 'k', 100.0);
                break;
            }
            case 2: {
                // Saturating
                cout << "\nHow fast does their affection grow?" << endl;
                cout << "1. Very Slow\n2. Slow\n3. Medium\n4. Fast\n5. Very Fast" << endl;
                cout << "Your choice (1-5): ";
                cin >> response;

                double logistic_GR_coeff = mapLogisticGrowthRate(response); 
                cout << "\n[Saturating Model] Calculating time to reach 95 affection from "
                     << affectionPoints << "..." << endl;
                // Placeholder for actual computation using affectionPoints and growth rate
                int idx = formulas.get_label_idx("Saturated");
                variable_dict.insert({'A', affectionPoints});
                variable_dict.insert({'K', 100.0});
                variable_dict.insert({'r', logistic_GR_coeff});
                ode_solver.request(formulas.get_function(idx), variable_dict, 'A', 95.0);
                break;
            }
            case 3: {
                // Avoidant
                cout << "\nHow avoidant is your partner?" << endl;
                cout << "1. Very Secure\n2. Mildly Avoidant\n3. Neutral\n4. Avoidant\n5. Extremely Avoidant" << endl;
                cout << "Your choice (1-5): ";
                cin >> response;
                cout << "\n[Avoidant Model] Calculating time to reach 100 affection from "
                     << affectionPoints << "..." << endl;
                // Placeholder for actual computation using affectionPoints and avoidant coefficient
                int idx = formulas.get_label_idx("Avoidant");
                variable_dict.insert({'A', affectionPoints});
                variable_dict.insert({'a', response});
                ode_solver.request(formulas.get_function(idx), variable_dict, 'A', 100.0);
                break;
            }
            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
        ode_solver.destroy();
    }

    cout << "\nThank you for using the Love Calculator. Goodbye!" << endl;
    return 0;
}
