#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <string>

using namespace std;

unordered_map<string, bool> neterminal;
unordered_map<char, bool> terminal;
string start_simbol;
unordered_map<string, set<string>> reguli;

void citire() {
    int n; cin >> n;
    for(int i = 0; i < n; ++i) {
        string nt; cin >> nt;
        neterminal[nt] = true;
    }
    cin >> n;
    for(int i = 0; i < n; ++i) {
        char t; cin >> t;
        terminal[t] = true;
    }
    cin >> start_simbol;

    cin >> n;
    for(int i = 0; i < n; ++i) {
        int nr_reguli; cin >> nr_reguli;
        string nt; cin >> nt;
        string sageata; cin >> sageata; 
        for(int j = 0; j < nr_reguli; ++j) {
            string rhs; cin >> rhs;
            if(rhs == "lambda" || rhs == "eps") rhs = "";
            reguli[nt].insert(rhs);
        }
    }
}

set<string> t[1001][1001];

void cyk(string s) {
    int n = s.size();
    s = "$" + s; // ca sa indexam stringul de la 1 la n

    // pasul 1
    for(int i = 1; i <= n; ++i) {
        // calculam t[i][1] = multimea neterminalelor care produc s[i]
        for(const auto & [nt, prod] : reguli) {
            for(auto x : prod) {
                if(x.size() == 1 && x[0] == s[i]) {
                    t[i][1].insert(nt);
                }
            }
        }
    }

    // pasul 2 (recursiv)
    for(int j = 2; j <= n; ++j) {
        for(int i = 1; i <= n - j + 1; ++i) {
            for(int k = 1; k < j; ++k) {
                for(const auto & B : t[i][k]) {
                    for(const auto & C : t[i + k][j - k]) {
                        string target = B + C;
                        for(const auto & [nt, prod] : reguli) {
                            if(prod.count(target)) {
                                t[i][j].insert(nt);
                            }
                        }
                    }
                }
            }
        }
    }

}

int main() {
    citire();
    string word;
    while(cin >> word) {
        int n = word.size();
        for(int i = 1; i <= n; ++i) {
            for(int j = 1; j <= n; ++j) {
                t[i][j].clear();
            }
        }

        cyk(word);

        auto aux = t[1][n];
        string tmp;
        tmp += start_simbol;
        int gasit = false;
        for(auto s : aux) {
            if(s == start_simbol) {
                cout << "cuvantul " << word << " poate fi generat de gramatica\n";
                gasit = true;
                break;
            }
        }

        if(!gasit) {
            cout << "cuvantul " << word << " nu poate fi generat de gramatica\n";
        }

    }
}