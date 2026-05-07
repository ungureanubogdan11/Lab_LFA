#include <fstream>
#include <vector>
#include <string>

using namespace std;

ifstream fin("inputs/data_lambda.in");
ofstream fout("outputs/answer_lambda.out");

int nr_stari, stare_init = 0, nr_litere, nr_stari_finale, nr_muchii;
bool stare_finala[1001];

string star(string r) {
    if (r == "") return "lambda";
    if (r == "lambda") return "lambda";
    if (r.length() == 1) return r + "*";
    return "(" + r + ")*";
}

string concat(string r1, string r2) {
    if (r1 == "" || r2 == "") return "";
    if (r1 == "lambda") return r2;
    if (r2 == "lambda") return r1;
    return r1 + r2;
}

string reunite(string r1, string r2) {
    if (r1 == "") return r2;
    if (r2 == "") return r1;
    if (r1 == r2) return r1;
    return "(" + r1 + "|" + r2 + ")";
}



void solve() {
    if (!(fin >> nr_stari >> nr_litere)) return;
    for (int i = 0; i < nr_litere; ++i) {
        string litera; fin >> litera;
    }
    
    fin >> nr_stari_finale;
    for (int i = 0; i < nr_stari_finale; ++i) {
        int stare; fin >> stare;
        stare_finala[stare] = 1;
    }
    
    fin >> nr_muchii;
    int MAX_STARI = nr_stari + 2; 
    vector<vector<string>> adj(MAX_STARI, vector<string>(MAX_STARI, ""));
    
    for (int i = 0; i < nr_muchii; ++i) {
        int src, dest;
        string lit;
        fin >> src >> dest >> lit;
        adj[src][dest] = reunite(adj[src][dest], lit);
    }
    
    int qs = nr_stari;
    int qf = nr_stari + 1;
    
    adj[qs][stare_init] = "lambda";
    for (int i = 0; i < nr_stari; ++i) {
        if (stare_finala[i]) {
            adj[i][qf] = "lambda";
        }
    }
    
    for (int k = 0; k < nr_stari; ++k) {
        for (int p = 0; p <= qf; ++p) {
            if (p == k || adj[p][k] == "") continue;
            for (int r = 0; r <= qf; ++r) {
                if (r == k || adj[k][r] == "") continue;
                
                string R_pk = adj[p][k];
                string R_kk = adj[k][k];
                string R_kr = adj[k][r];
                string path_through_k = concat(R_pk, concat(star(R_kk), R_kr));
                
                adj[p][r] = reunite(adj[p][r], path_through_k);
            }
        }
        for (int i = 0; i <= qf; ++i) {
            adj[k][i] = "";
            adj[i][k] = "";
        }
    }
    
    fout << adj[qs][qf] << endl;
}

int main() {
    solve();
    return 0;
}