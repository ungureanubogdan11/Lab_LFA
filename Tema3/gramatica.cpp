#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <set>

using namespace std;

ifstream cin("inputs/date_gramatica.in");
ofstream cout("outputs/date_gramatica.out");

unordered_map<char, int> neterminal;
unordered_map<char, int> terminal;
char start;
vector<string> reguli[30];
int k;

set<string> cuvinte;

void citire_date() {
    int n; cin >> n;
    for(int i = 0; i < n; ++i) {
        char c; cin >> c;
        neterminal[c] = 1;
    }
    cin >> n;
    for(int i = 0; i < n; ++i) {
        char c; cin >> c;
        terminal[c] = 1;
    }

    cin >> start;

    // reguli de productie de forma nr_reguli_prod A -> alfa | beta

    cin >> n;
    for(int i = 0; i < n; ++i) {
        int nr_reguli; cin >> nr_reguli;
        char c; cin >> c;
        c -= 'A';
        string s; cin >> s; // s = "->"
        for(int j = 0; j < nr_reguli; ++j) {
            cin >> s;
            if(s == "lambda") reguli[c].push_back("");
            else reguli[c].push_back(s);
        }
    }

    cin >> k;
}

void gen_cuv(int k, string s) {
    int nr_terminale = 0;
    int primul_neterminal_idx = -1;

    for(int i = 0; i < s.size(); ++i) {
        if(neterminal[s[i]]) {
            if(primul_neterminal_idx == -1) {
                primul_neterminal_idx = i;
            }
        } else if(terminal[s[i]]) {
            nr_terminale++;
        }
    }

    if(nr_terminale > k) {
        return;
    }

    if(primul_neterminal_idx == -1) {
        if(s.size() == k) {
            cuvinte.insert(s);
        }
        return;
    }

    
    char idx = s[primul_neterminal_idx] - 'A';
    for(int j = 0; j < reguli[idx].size(); ++j) {
        string new_s = s.substr(0, primul_neterminal_idx);
        new_s += reguli[idx][j];
        new_s += s.substr(primul_neterminal_idx + 1);
        
        if(new_s.size() <= k + 10) {
            gen_cuv(k, new_s);
        }
    }

}

void afisare() {
    for(auto it = cuvinte.begin(); it != cuvinte.end(); it++) {
        cout << *it << '\n';
    }
}

int main() {
    citire_date();
    
    string tmp; 
    tmp.push_back(start);
    gen_cuv(k, tmp);

    afisare();
}