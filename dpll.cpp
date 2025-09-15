#include "dpll.h"
#include <cmath>   

// Chooses the next unassigned variable and assigns it a value 0 initially
// Returns false if all variables are already assigned
bool decide(unordered_map<int, int>& assignments, stack<Decision>& decisionStack, int& currentDecisionLevel) {
    for (auto& [var, val] : assignments) {
        if(val == -1) {
            assignments[var] = 0;
            decisionStack.push({var, 0, false, ++currentDecisionLevel});
            return true;
        }
    }
    return false;
}

// Propagates unit clauses using BCP
// Returns false if a conflict is detected, true otherwise
bool bcp(const vector<vector<int>>& cnf, unordered_map<int, int>& assignments, stack<Decision>& decisionStack, int& currentDecisionLevel) {
    bool changed = true;
    while(changed) {
        changed = false;
        for(const auto& clause : cnf) {
            int unassignedCount = 0;
            int lastUnassignedLit = 0;
            bool clauseSatisfied = false;

            // checks if the clause is already sat and counts the number of unassigned literals and saves the last unassigned literal
            for(int lit : clause) {
                int var = abs(lit);
                int val = assignments[var];
                if(val == -1) {
                    unassignedCount++;
                    lastUnassignedLit = lit;
                }
                else if((val == 1 && lit > 0) || (val == 0 && lit < 0)) {
                    clauseSatisfied = true;
                    break;
                }
            }
            
            // all literals are already set to 0 so there is a conflict
            if(!clauseSatisfied && unassignedCount == 0) return false;

            // if all literals are 0 except one
            if(!clauseSatisfied && unassignedCount == 1) {
                int var = abs(lastUnassignedLit);
                int val = (lastUnassignedLit > 0) ? 1 : 0;

                if(assignments[var] == -1) {
                    assignments[var] = val;
                    decisionStack.push({var, val, true, currentDecisionLevel});
                    changed = true;
                }
                changed = true;
            }
        }
    }
    return true; // no conflicts
}

// Resolves conflicts by backtracking to previous decisions
// Returns false if no decisions left to flip (UNSAT)
bool resolveConflict(unordered_map<int,int>& assignments,
                     stack<Decision>& decisionStack,
                     int& currentDecisionLevel) {
    while(!decisionStack.empty()) {
        Decision d = decisionStack.top();
        decisionStack.pop();

        assignments[d.var] = -1;

        if(!d.triedBoth) {
            d.value = 1 - d.value;
            d.triedBoth = true;

            assignments[d.var] = d.value;
            decisionStack.push(d);
            currentDecisionLevel = d.decisionLevel;
            return true;
        }
    }
    return false;
}

// Main DPLL solver: decide, propagate, and backtrack
// Returns true if SAT, false if UNSAT
bool dpll(const vector<vector<int>>& cnf, unordered_map<int, int>& assignments, stack<Decision>& decisionStack, int& currentDecisionLevel) {
    while(true) {
        if(!decide(assignments, decisionStack, currentDecisionLevel)) {
            return true;
        }
        while(!bcp(cnf, assignments, decisionStack, currentDecisionLevel)) {
            if(!resolveConflict(assignments, decisionStack, currentDecisionLevel)) {
                return false;
            }
        }
    }
}

