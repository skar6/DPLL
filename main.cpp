#include "dpll.h"
#include <iostream>


int main() {
    vector<vector<int>> cnf = {
        {1},
        {-1},
    };
    vector<int> variables = {1, 3};
    unordered_map<int,int> assignments;
    stack<Decision> decisionStack;
    int currentDecisionLevel = 0;

    for(int var : variables) assignments[var] = -1;

    bool sat = dpll(cnf, assignments, decisionStack, currentDecisionLevel);

    if(sat) {
        cout << "SATISFIABLE\nAssignments:\n";
        for(int var : variables) {
            cout << "x" << var << " = " << assignments[var] << "\n";
        }
    } else {
        cout << "UNSATISFIABLE\n";
    }

    return 0;
}
