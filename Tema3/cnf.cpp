#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>

using namespace std;

ifstream cin("inputs/date_gramatica.in");
ofstream cout("outputs/date_cnf.out");

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

void pasul_start() {

    // verific toate regulile vad daca s apara in dreapta unei reguli

    bool apare_in_dreapta = false;
    for(auto const& [stanga, dreapta_set] : reguli) {
        for(const string& alternativa : dreapta_set) {
            if(alternativa.find(start_simbol) != string::npos) {
                apare_in_dreapta = true;
                break;
            }
        }
    }
    if(apare_in_dreapta) {
        string nou_start = start_simbol + "0";
        neterminal[nou_start] = true;
        reguli[nou_start].insert(start_simbol);
        start_simbol = nou_start;
    }
}

void pasul_term() {
    unordered_map<char, string> terminal_to_nt;
    unordered_map<string, set<string>> reguli_noi;

    for(auto const& [nt, alternative] : reguli) {
        for(string alt : alternative) {
            if(alt.size() == 1 && terminal[alt[0]]) {
                reguli_noi[nt].insert(alt);
                continue;
            }
            
            string alt_modificat = "";
            for(char c : alt) {
                if(terminal[c]) {
                    string noul_nt = "X_";
                    noul_nt += c;
                    neterminal[noul_nt] = true;
                    terminal_to_nt[c] = noul_nt;
                    alt_modificat += noul_nt; 
                } else {
                    alt_modificat += c;
                }
            }
            reguli_noi[nt].insert(alt_modificat);
        }
    }

    for(auto const& [term, nt_nou] : terminal_to_nt) {
        string s = ""; s += term;
        reguli_noi[nt_nou].insert(s);
    }
    reguli = reguli_noi;
}

void pasul_bin() {
    unordered_map<string, set<string>> reguli_noi;
    int contor_c = 1;

    for(auto const& [nt, alternative] : reguli) {
        for(string alt : alternative) {
            if(alt.size() > 2) {
                string curent_nt = nt;
                for(size_t i = 0; i < alt.size() - 2; ++i) {
                    string nou_neterminal = "C" + to_string(contor_c++);
                    neterminal[nou_neterminal] = true;
                    
                    string rhs = "";
                    rhs += alt[i];
                    rhs += nou_neterminal;
                    reguli_noi[curent_nt].insert(rhs);
                    
                    curent_nt = nou_neterminal;
                }
                string final_rhs = "";
                final_rhs += alt[alt.size() - 2];
                final_rhs += alt[alt.size() - 1];
                reguli_noi[curent_nt].insert(final_rhs);
            } else {
                reguli_noi[nt].insert(alt);
            }
        }
    }
    reguli = reguli_noi;
}

void pasul_del() {
    set<string> anulabile;
    bool schimbare = true;

    for(auto const& [nt, alternative] : reguli) {
        if(alternative.count("")) {
            anulabile.insert(nt);
        }
    }

    while(schimbare) {
        schimbare = false;
        for(auto const& [nt, alternative] : reguli) {
            if(anulabile.count(nt)) continue;
            for(string alt : alternative) {
                bool toate_anulabile = true;
                for(char c : alt) {
                    string simb(1, c);
                    if(!anulabile.count(simb)) toate_anulabile = false;
                }
                if(toate_anulabile && !alt.empty()) {
                    anulabile.insert(nt);
                    schimbare = true;
                }
            }
        }
    }

    unordered_map<string, set<string>> reguli_noi;
    for(auto const& [nt, alternative] : reguli) {
        for(string alt : alternative) {
            if(alt.empty()) continue; 
            
            vector<string> combinatii = {""};
            for(char c : alt) {
                string simb(1, c);
                int len = combinatii.size();
                for(int i = 0; i < len; ++i) {
                    if(anulabile.count(simb)) {
                        combinatii.push_back(combinatii[i] + simb); 
                    }
                    combinatii[i] += simb; 
                }
            }
            for(string comb : combinatii) {
                if(!comb.empty()) reguli_noi[nt].insert(comb);
            }
        }
    }
    reguli = reguli_noi;
}

void pasul_unit() {
    unordered_map<string, set<string>> reguli_noi = reguli;
    
    for(auto& [nt, alternative] : reguli_noi) {
        alternative.erase(nt);
    }

    bool schimbare = true;
    while(schimbare) {
        schimbare = false;
        for(auto& [nt, alternative] : reguli_noi) {
            set<string> de_adaugat;
            for(string alt : alternative) {
                if(alt.size() == 1 && neterminal[alt]) {
                    for(string prod_b : reguli_noi[alt]) {
                        if(reguli_noi[nt].find(prod_b) == reguli_noi[nt].end()) {
                            de_adaugat.insert(prod_b);
                            schimbare = true;
                        }
                    }
                }
            }
            for(string nou : de_adaugat) {
                reguli_noi[nt].insert(nou);
            }
        }
    }

    for(auto& [nt, alternative] : reguli_noi) {
        set<string> curat;
        for(string alt : alternative) {
            if(!(alt.size() == 1 && neterminal[alt])) {
                curat.insert(alt);
            }
        }
        reguli_noi[nt] = curat;
    }
    reguli = reguli_noi;
}

void afisare_cnf() {
    cout << "Simbol de start: " << start_simbol << "\n\nReguli CNF:\n";
    for(auto const& [nt, alternative] : reguli) {
        if(alternative.empty()) continue;
        cout << nt << " -> ";
        bool primul = true;
        for(string alt : alternative) {
            if(!primul) cout << " | ";
            cout << (alt.empty() ? "lambda" : alt);
            primul = false;
        }
        cout << "\n";
    }
}

int main() {
    citire();
    
    pasul_start();
    pasul_term();
    pasul_bin();
    pasul_del();
    pasul_unit();
    
    afisare_cnf();
    return 0;
}