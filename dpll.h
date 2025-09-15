#ifndef DPLL_H
#define DPLL_H

#include <vector>
#include <unordered_map>
#include <stack>

using namespace std;

// Decision struct for tracking variable assignments
struct Decision {
    int var;            
    int value;          
    bool triedBoth;     
    int decisionLevel;  
};

// Function declarations

bool dpll(const vector<vector<int>>& cnf,
          unordered_map<int,int>& assignments,
          stack<Decision>& decisionStack,
          int& currentDecisionLevel);

bool decide(unordered_map<int,int>& assignments,
            stack<Decision>& decisionStack,
            int& currentDecisionLevel);

bool bcp(const vector<vector<int>>& cnf,
         unordered_map<int,int>& assignments,
         stack<Decision>& decisionStack,
         int& currentDecisionLevel);

bool resolveConflict(unordered_map<int,int>& assignments,
                     stack<Decision>& decisionStack,
                     int& currentDecisionLevel);

#endif
