#include "dpll.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>


vector<vector<int>> readCNF(const string& filename) {
    ifstream infile(filename);
    if(!infile) {
        cerr << "Error: Could not open " << filename << "\n";
        exit(1);
    }
    
    vector<vector<int>> cnf;
    vector<int> clause;
    string line;
    int numVars = 0, numClauses = 0;

    while(getline(infile, line)) {
        if(line.empty()) continue;
        if(line[0] == 'c') continue;
        if(line[0] == 'p') {
            break;
        }
    }

    int lit;
    while (infile >> lit) {
        if (lit == 0) {
            cnf.push_back(clause);
            clause.clear();
        } else {
            clause.push_back(lit);
        }
    }

    cout << "Parsed CNF clauses:\n";
    for (size_t i = 0; i < cnf.size(); i++) {
        cout << "Clause " << i+1 << ": ";
        for (int lit : cnf[i]) {
            cout << lit << " ";
        }
        cout << "\n";
    }

    return cnf;

}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <cnf-file>\n";
        return 1;
    }

    // Read CNF file
    vector<vector<int>> cnf = readCNF(argv[1]);

    // Collect all variables (unique, no duplicates)
    unordered_set<int> varSet;
    for (const auto& clause : cnf) {
        for (int lit : clause) {
            varSet.insert(abs(lit));
        }
    }
    vector<int> variables(varSet.begin(), varSet.end());

    // Initialize assignments to -1 (unassigned)
    unordered_map<int,int> assignments;
    for (int var : variables) {
        assignments[var] = -1;
    }

    stack<Decision> decisionStack;
    int currentDecisionLevel = 0;

    // Run DPLL
    bool sat = dpll(cnf, assignments, decisionStack, currentDecisionLevel);
    cout << "\nSAT SOLVER RESULT: ";
    if(sat) {
        cout << "\n\tSATISFIABLE\n\tAssignments:\n";
        for (int var : variables) {
            cout << "\tx" << var << " = " << assignments[var] << "\n";
        }
    } else {
        cout << "\n\tUNSATISFIABLE\n";
    }


    return 0;
}