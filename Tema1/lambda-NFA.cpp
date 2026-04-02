#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>

using namespace std;

ifstream cin("inputs/data_lambda.in");
ofstream cout("outputs/answer_lambda.out");

int nr_stari;
int stare_init = 0;
int nr_litere;
int nr_stari_finale;
bool stare_finala[1001];

int nr_muchii;
struct muchie {
    int src, dest;
    string lit;
};
muchie muchii[1001];

int nr_words;
string words[1001];
vector<pair<string, int>> G[1001];
vector<int> lambda_graph[1001];

void read_input() {
    if (!(cin >> nr_stari >> nr_litere)) return;
    for (int i = 0; i < nr_litere; ++i) {
        string litera;
        cin >> litera;
    }
    cin >> nr_stari_finale;
    for (int i = 0; i < nr_stari_finale; ++i) {
        int stare; cin >> stare;
        stare_finala[stare] = 1;
    }
    cin >> nr_muchii;
    for (int i = 0; i < nr_muchii; ++i) {
        cin >> muchii[i].src >> muchii[i].dest >> muchii[i].lit;
    }
    cin >> nr_words;
    for (int i = 0; i < nr_words; ++i) {
        cin >> words[i];
    }
}

void gen_graph() {
    for (int i = 0; i < nr_muchii; ++i) {
        if (muchii[i].lit == "lambda") {
            lambda_graph[muchii[i].src].push_back(muchii[i].dest);
        } else {
            G[muchii[i].src].push_back({muchii[i].lit, muchii[i].dest});
        }
    }
}

vector<int> lambda_closure(int stage) {
    vector<int> ans;
    queue<int> q;
    unordered_map<int, bool> visited;
    
    q.push(stage);
    visited[stage] = true;
    ans.push_back(stage);

    while (!q.empty()) {
        int nod = q.front();
        q.pop();
        for (int next_node : lambda_graph[nod]) {
            if (!visited[next_node]) {
                visited[next_node] = true;
                ans.push_back(next_node);
                q.push(next_node);
            }
        }
    }
    return ans;
}

bool check_word(string word, int index, int stare) {
    // if(index == 0) {
    //     cout << "lambda";
    // }
    // for(int i = 0; i < index; ++i) {
    //     cout << word[i];
    // }
    // cout << ' ' << stare;
    // cout << '\n';

    vector<int> closure = lambda_closure(stare);


    if (index == word.size()) {
        for (int s : closure) {
            if (stare_finala[s]) return true; 
        }
        return false;
    }

    for (int s : closure) {
        for (auto edge : G[s]) {
            string lit = edge.first;
            int next_node = edge.second;

            if (index + lit.size() <= word.size() && word.substr(index, lit.size()) == lit) {
                if (check_word(word, index + lit.size(), next_node)) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    read_input();
    gen_graph();

    for (int i = 0; i < nr_words; ++i) {
        if (check_word(words[i], 0, stare_init)) {
            cout << "Cuvantul " << words[i] << " apartine limbajului!\n";
        } else {
            cout << "Cuvantul " << words[i] << " NU apartine limbajului!\n";
        }
    }

    return 0;
}