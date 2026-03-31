#include <fstream>
#include <unordered_map>

using namespace std;

ifstream cin("inputs/data_nfa.in");
ofstream cout("outputs/answer_nfa.out");

int nr_stari;
int stare_init = 0;
int nr_litere;
unordered_map<string, bool> is_alfabet;
unordered_map<string, bool> is_prefix; 
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
vector<pair<string, int>> G[1001]; // graph automata
int operation; // 1 means check words, 2 means print all words, 3 means print all words of length L
int length_of_words; // for third operation
int printed_words = 0;

void read_input()
{
    cin >> nr_stari;
    cin >> nr_litere;
    for(int i = 0; i < nr_litere; ++i) {
        string litera;
        cin >> litera;
        string prefix;
        for(int i = 0; i < litera.size(); ++i) {
            prefix.push_back(litera[i]);
            is_prefix[prefix] = 1;    
        }
        is_alfabet[litera] = 1;
    }
    cin >> nr_stari_finale;
    for(int i = 0; i < nr_stari_finale; ++i) {
        int stare; cin >> stare;
        stare_finala[stare] = 1;
    }
    
    cin >> nr_muchii;
    for(int i = 0; i < nr_muchii; ++i) {
        cin >> muchii[i].src >> muchii[i].dest >> muchii[i].lit;
    }

    cin >> operation;
    if(operation == 1) {
        cin >> nr_words;
        for(int i = 0; i < nr_words; ++i) {
            cin >> words[i];
        }
    }
    else if(operation == 3) {
        cin >> length_of_words;
    }
}

void gen_graph()
{
    for(int i = 0; i < nr_muchii; ++i) {
        auto muchie = muchii[i];
        G[muchie.src].push_back({muchie.lit, muchie.dest});
    }
}

pair<string, int> find_letter(string word, int index)
{
    string letter, ans_letter;
    int ans_index = index;
    for(int i = index; i < word.size(); ++i) {
        letter.push_back(word[i]);
        if(is_alfabet[letter]) {
            ans_letter = letter;
            ans_index = i;
        }
        if(!is_prefix[letter]) {
            return {ans_letter, ans_index};
        }
        
    }
    return {ans_letter, ans_index};
}

bool check_word(string word, int index, int stare, vector<int> stari = vector<int>(1, 1))
{
    cout << "Cazul ";
    if(word == "lambda") {
        cout << "1: ";
        cout << word << ' ' << stare << '\n';
        return stare_finala[stare];
    }
    for(int i = 0; i < stari.size(); ++i) {
        cout << stari[i];
        if(i != stari.size() - 1) cout << ".";
    }
    cout << ": ";
    if(index == 0) cout << "lambda";
    for(int i = 0; i < index; ++i) cout << word[i];
    cout << ' ' << stare << '\n';
    if(index == word.size()) {
        return stare_finala[stare];
    }
    else if(index > word.size()) {
        return 0;
    }

    string letter;
    int new_index;
    auto x = find_letter(word, index);
    letter = x.first, new_index = x.second;
    if(letter.size() == 0) return 0;

    bool ans = 0;
    int count_stari = 0;
    for(auto nodnou : G[stare]) {
        if(letter == nodnou.first) {
            count_stari++;
            if(stari.empty() || count_stari != stari[stari.size() - 1]) stari.push_back(count_stari);
            ans = max(ans, check_word(word, new_index + 1, nodnou.second, stari));
        }
    }
    return ans;

}

void print_all_words(int length = -1, string word = "", int stare = 0) // not exactly all words
{
    if(length != -1 && word.size() > length) return;
    if(printed_words > 100) return;
    if(stare_finala[stare]) {
        if(length == -1 || length == word.size()) {
            if(word.size() == 0) {
                cout << "lambda\n";
            }
            else {
                cout << word + "\n";
            }
            printed_words++;
        }
    }

    for(auto nodnou : G[stare]) {
        int new_stare = nodnou.second;
        string new_word = word + nodnou.first;
        print_all_words(length, new_word, new_stare);
    }
}    



int main()
{
    read_input();
    
    gen_graph();

    
    if(operation == 1) {
        for(int i = 0; i < nr_words; ++i) {
            vector<int> tmp;
            if(check_word(words[i], 0, stare_init)) {
                cout << "Cuvantul " + words[i] + " apartine limbajului!\n";
            }
            else {
                cout << "Cuvantul " + words[i] + " NU apartine limbajului!\n";
            }
        }
    }
    else if(operation == 2) {
        print_all_words();
    }
    else if(operation == 3) {
        print_all_words(length_of_words);
    }


}