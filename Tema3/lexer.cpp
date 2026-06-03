#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

using namespace std;

ifstream cin("inputs/date_lexer.in");
ofstream cout("outputs/date_lexer.out");

struct Token {
    string type;
    string value;
};

const unordered_set<string> KEYWORDS = {
    "if", "else", "while", "int", "return"
};

bool isOperatorChar(char ch) {
    string ops = "+-*/=";
    return ops.find(ch) != string::npos;
}

vector<Token> tokenize(const string& source) {
    vector<Token> tokens;
    size_t i = 0;
    size_t length = source.length();

    while (i < length) {
        char ch = source[i];

        if (isspace(ch)) {
            i++;
            continue;
        }

        if (isalpha(ch) || ch == '_') {
            string start_word = "";
            while (i < length && (isalnum(source[i]) || source[i] == '_')) {
                start_word += source[i];
                i++;
            }

            if (KEYWORDS.count(start_word)) {
                tokens.push_back({"KEYWORD", start_word});
            } else {
                tokens.push_back({"IDENTIFIER", start_word});
            }
            continue;
        }

        if (isdigit(ch)) {
            string num = "";
            while (i < length && isdigit(source[i])) {
                num += source[i];
                i++;
            }
            tokens.push_back({"LITERAL_NUM", num});
            continue;
        }

        if (ch == '"' || ch == '\'') {
            char quote_type = ch;
            string literal = "";
            literal += ch;
            i++;

            while (i < length && source[i] != quote_type) {
                literal += source[i];
                i++;
            }

            if (i < length) {
                literal += source[i];
                i++;
            }
            tokens.push_back({"LITERAL_STR", literal});
            continue;
        }

        if (isOperatorChar(ch)) {
            string op = "";
            op += ch;
            i++;

            if (ch == '=' && i < length && source[i] == '=') {
                op += source[i];
                i++;
            }

            tokens.push_back({"OPERATOR", op});
            continue;
        }

        i++;
    }

    return tokens;
}

int main() {
    string codSursa;
    string s; 
    while(getline(cin, s)) {
        codSursa += s;
    }


    vector<Token> listaTokeni = tokenize(codSursa);

    for (const auto& token : listaTokeni) {
        cout << "(" << token.type << ", " << token.value << ")\n";
    }

    return 0;
}