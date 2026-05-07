#include <fstream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

ifstream fin("inputs/data_pda.in");
ofstream fout("outputs/answer_pda.out");

struct Tranzitie {
    int stare_destinatie;
    string caractere_noi_stiva;
};

struct Muchie {
    char simbol_intrare;
    char simbol_top_stiva;
    vector<Tranzitie> optiuni;
};

int nr_stari, stare_start, nr_finale;
bool este_finala[1001];
char simbol_initial_stiva;
vector<Muchie> muchii[1001];

bool ruleaza_pda(int stare_ac, int poz_cuvant, string cuvant, stack<char> stiva) {
    if (poz_cuvant == cuvant.length()) {
        if (este_finala[stare_ac]) return true;
        if (stiva.empty()) return true;
    }

    for (auto& m : muchii[stare_ac]) {
        bool poate_citi = (poz_cuvant < cuvant.length() && m.simbol_intrare == cuvant[poz_cuvant]);
        bool este_lambda = (m.simbol_intrare == 'L');

        if (poate_citi || este_lambda) {
            if (!stiva.empty() && stiva.top() == m.simbol_top_stiva) {
                for (auto& opt : m.optiuni) {
                    stack<char> stiva_noua = stiva;
                    stiva_noua.pop();

                    if (opt.caractere_noi_stiva != "L") {
                        for (int i = opt.caractere_noi_stiva.length() - 1; i >= 0; i--) {
                            stiva_noua.push(opt.caractere_noi_stiva[i]);
                        }
                    }

                    int urmatoarea_pozitie = poate_citi ? poz_cuvant + 1 : poz_cuvant;
                    if (ruleaza_pda(opt.stare_destinatie, urmatoarea_pozitie, cuvant, stiva_noua)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    if (!(fin >> nr_stari >> stare_start >> simbol_initial_stiva)) return 0;
    
    fin >> nr_finale;
    for (int i = 0; i < nr_finale; i++) {
        int f; fin >> f;
        este_finala[f] = true;
    }
    
    int nr_muchii;
    fin >> nr_muchii;
    for (int i = 0; i < nr_muchii; i++) {
        int u, v; char in, top; string rez;
        fin >> u >> in >> top >> v >> rez;
        muchii[u].push_back({in, top, {{v, rez}}});
    }

    string cuvant_test;
    while (fin >> cuvant_test) {
        stack<char> s;
        s.push(simbol_initial_stiva); 
        
        if (ruleaza_pda(stare_start, 0, cuvant_test, s)) fout << "DA\n";
        else fout << "NU\n";
    }
    return 0;
}