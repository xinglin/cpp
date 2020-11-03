#include <iostream>
#include <vector>

using namespace std;

struct TrieNode {
    bool isEnd;
    TrieNode* next[26];
    TrieNode() {
        isEnd = false;
        for(int i=0; i < 26; i++)
            next[i] = nullptr;
    }
};

class Trie {

    TrieNode* root;

public:

    Trie() {
        root = new TrieNode();
    }

    TrieNode* getRoot() {
        return root;
    }

    void addWord(string word) {
        TrieNode* cur = root;

        for(char c: word) {
            if(cur->next[c-'a']==nullptr) {
                TrieNode* n = new TrieNode();
                cur->next[c-'a'] = n;
            }

            cur = cur->next[c-'a'];
        }

        cur->isEnd = true;
    } 

};


class PhoneNumDict {
    static const vector<vector<char>> numMap;
    Trie trie;
    int debug=1;
public:
    PhoneNumDict(vector<string>& dict) {
        for(string & w: dict) {
            if(debug)
                std::cout << "addWord: " << w << std::endl;
            trie.addWord(w);
        }
        if(debug)
            std::cout << "finish adding words to trie\n"; 
    }
    TrieNode* getRoot() {
        return trie.getRoot();
    }

    void checkWord(string str, int i, TrieNode* root, string& word, vector<string>& words) {
        if(i == str.size()) {
            words.push_back(word);
            return;
        }
        
        if(root->isEnd) {
            words.push_back(word);
        }

        const vector<char>& charset = numMap[str[i]-'0'];
        for(auto& c: charset) {
            if(root->next[c-'a']!=nullptr) {
                word.push_back(c);
                checkWord(str, i+1, root->next[c-'a'], word, words);
                word.pop_back();
            }
        }
    }

};

const vector<vector<char>> PhoneNumDict::numMap = {{}, {}, {'a','b','c'}, {'d','e','f'}, 
            {'g','h','i'}, {'j','k','l'}, {'m','n','o'}, {'p','q','r','s'},
            {'t','u','v'}, {'w','x','y','z'}};


int main() {

    vector<string> dict = {"xing", "mnmn", "lin", "utah", "nankai", "california"};
    PhoneNumDict phonenum(dict);

    vector<string> res;
    string s;
    phonenum.checkWord("9464", 0, phonenum.getRoot(), s, res);
    for(string& s: res) {
        std::cout << s << std::endl;
    }
}