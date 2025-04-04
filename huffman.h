#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

struct Node {
    char data;
    int freq;
    Node *left, *right;
    Node(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void buildHuffmanTree(const string &data, unordered_map<char, string> &codes, unordered_map<string, char> &reverseCodes, Node* &root);
string encode(const string &data, unordered_map<char, string> &codes);
string decode(const string &encodedData, unordered_map<string, char> &reverseCodes);
void saveHuffmanCodes(const string &filePath, unordered_map<char, string> &codes);
bool loadHuffmanCodes(const string &filePath, unordered_map<char, string> &codes, unordered_map<string, char> &reverseCodes);

#endif