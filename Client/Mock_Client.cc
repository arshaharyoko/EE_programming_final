// --- mockclient.cc ---
#include "Solver_Client.hh"
#include "json_parser.hh"
#include <iostream>
#include <string>
#include <map>

using namespace std;

double mapLogisticGrowthRate(int rating) {
    double r;
    switch(rating) {
        case 1: r = 0.02; break;
        case 2: r = 0.05; break;
        case 3: r = 0.10; break;
        case 4: r = 0.20; break;
        case 5: r = 0.30; break;
        default:
            cout << "Invalid input. Using default (0.10).\n";
            r = 0.10;
    }
    return r;
}

int mapAffection(int rating) {
    switch (rating) {
        case 1: return 10;
        case 2: return 25;
        case 3: return 50;
        case 4: return 75;
        case 5: return 90;
        default: return 0;
    }
}

string interpretTime(int days) {
    if (days < 0)
        return "Tidak bisa diprediksi. Coba lagi.";
    if (days == 0)
        return "Langsung hubungi sekarang!";
    if (days <= 2)
        return "Tunggu dua hari.";
    if (days <= 7)
        return "Tunggu beberapa hari.";
    if (days <= 30)
        return "Bersabarlah, mungkin butuh beberapa minggu.";
    return "Mulai menghindar.";
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
        variable_dict = {};
        cout << "\n--- Love Calculator ---" << endl;
        cout << "1. Linear Partner" << endl;
        cout << "2. Saturating Partner" << endl;
        cout << "3. Avoidant Partner" << endl;
        cout << "4. Exit" << endl;
        cout << "Choose your partner type (1-4): ";
        cin >> choice;

        if(choice == 4) break;

        int rating = 0;
        cout << "\nRate current affection level:" << endl;
        cout << "1. Very Low\n2. Low\n3. Neutral\n4. High\n5. Very High" << endl;
        cout << "Your choice (1-5): ";
        cin >> rating;
        int affectionPoints = mapAffection(rating);
        cout << "Current affection points: " << affectionPoints << "/100" << endl;

        int response, days;
        switch (choice) {
            case 1: {
                cout << "\n[Linear Model] Calculating time to reach 100 affection from "
                << affectionPoints << "..." << endl;
                int idx = formulas.get_label_idx("Linear");
                variable_dict.insert({'k', 0.2});
                variable_dict.insert({'A', affectionPoints});
                days = ode_solver.request(formulas.get_function(idx), variable_dict, 'A', 100.0);
                cout << interpretTime(days) << endl;
                break;
            }
            case 2: {
                cout << "\nHow fast does their affection grow?" << endl;
                cout << "1. Very Slow\n2. Slow\n3. Medium\n4. Fast\n5. Very Fast" << endl;
                cout << "Your choice (1-5): ";
                cin >> response;
                double logistic_GR_coeff = mapLogisticGrowthRate(response);
                cout << "\n[Saturating Model] Calculating time to reach 95 affection from "
                << affectionPoints << "..." << endl;
                int idx = formulas.get_label_idx("Saturated");
                variable_dict.insert({'A', affectionPoints});
                variable_dict.insert({'K', 100.0});
                variable_dict.insert({'r', logistic_GR_coeff});
                days = ode_solver.request(formulas.get_function(idx), variable_dict, 'A', 95.0);
                cout << interpretTime(days) << endl;
                break;
            }
            case 3: {
                cout << "\nHow avoidant is your partner?" << endl;
                cout << "1. Very Secure\n2. Mildly Avoidant\n3. Neutral\n4. Avoidant\n5. Extremely Avoidant" << endl;
                cout << "Your choice (1-5): ";
                cin >> response;
                cout << "\n[Avoidant Model] Calculating time to reach 50 affection from "
                << affectionPoints << "..." << endl;
                int idx = formulas.get_label_idx("Avoidant");
                variable_dict.insert({'A', affectionPoints});
                variable_dict.insert({'a', response});
                days = ode_solver.request(formulas.get_function(idx), variable_dict, 'A', 50.0);
                cout << interpretTime(days) << endl;
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
