#include "huffman.h"
#include <functional>

using namespace std;

void buildHuffmanTree(const string &data, unordered_map<char, string> &codes, unordered_map<string, char> &reverseCodes, Node* &root) {
    unordered_map<char, int> freq;
    for (char ch : data) freq[ch]++;
    
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto &[ch, f] : freq) pq.push(new Node(ch, f));
    
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *parent = new Node('\0', left->freq + right->freq);
        parent->left = left; parent->right = right;
        pq.push(parent);
    }
    
    root = pq.top();
    function<void(Node*, string)> generateCodes = [&](Node* node, string str) {
        if (!node) return;
        if (node->data != '\0') {
            codes[node->data] = str;
            reverseCodes[str] = node->data;
        }
        generateCodes(node->left, str + "0");
        generateCodes(node->right, str + "1");
    };
    generateCodes(root, "");
}

void saveHuffmanCodes(const string &filePath, unordered_map<char, string> &codes) {
    ofstream file(filePath);
    for (auto &[ch, code] : codes) {
        file << ch << " " << code << "\n";
    }
    file.close();
}

bool loadHuffmanCodes(const string &filePath, unordered_map<char, string> &codes, unordered_map<string, char> &reverseCodes) {
    ifstream file(filePath);
    if (!file) {
        return false;
    }
    
    char ch;
    string code;
    while (file >> ch >> code) {
        codes[ch] = code;
        reverseCodes[code] = ch;
    }
    file.close();
    
    return !codes.empty();
}

string encode(const string &data, unordered_map<char, string> &codes) {
    string encodedStr;
    for (char ch : data) {
        encodedStr += codes[ch];
    }
    return encodedStr;
}