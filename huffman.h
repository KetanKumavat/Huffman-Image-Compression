#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <fstream>

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

void buildHuffmanTree(const std::string &data, std::unordered_map<char, std::string> &codes, std::unordered_map<std::string, char> &reverseCodes, Node* &root);
std::string encode(const std::string &data, std::unordered_map<char, std::string> &codes);
std::string decode(const std::string &encodedData, std::unordered_map<std::string, char> &reverseCodes);
void saveHuffmanCodes(const std::string &filePath, std::unordered_map<char, std::string> &codes);
bool loadHuffmanCodes(const std::string &filePath, std::unordered_map<char, std::string> &codes, std::unordered_map<std::string, char> &reverseCodes);

#endif