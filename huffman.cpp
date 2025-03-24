#include "huffman.h"
#include <functional>

void buildHuffmanTree(const std::string &data, std::unordered_map<char, std::string> &codes, std::unordered_map<std::string, char> &reverseCodes, Node* &root) {
    std::unordered_map<char, int> freq;
    for (char ch : data) freq[ch]++;
    
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto &[ch, f] : freq) pq.push(new Node(ch, f));
    
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *parent = new Node('\0', left->freq + right->freq);
        parent->left = left; parent->right = right;
        pq.push(parent);
    }
    
    root = pq.top();
    std::function<void(Node*, std::string)> generateCodes = [&](Node* node, std::string str) {
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

void saveHuffmanCodes(const std::string &filePath, std::unordered_map<char, std::string> &codes) {
    std::ofstream file(filePath);
    for (auto &[ch, code] : codes) {
        file << ch << " " << code << "\n";
    }
    file.close();
}

bool loadHuffmanCodes(const std::string &filePath, std::unordered_map<char, std::string> &codes, std::unordered_map<std::string, char> &reverseCodes) {
    std::ifstream file(filePath);
    if (!file) {
        return false;
    }
    
    char ch;
    std::string code;
    while (file >> ch >> code) {
        codes[ch] = code;
        reverseCodes[code] = ch;
    }
    file.close();
    
    return !codes.empty();
}

std::string encode(const std::string &data, std::unordered_map<char, std::string> &codes) {
    std::string encodedStr;
    for (char ch : data) {
        encodedStr += codes[ch];
    }
    return encodedStr;
}

std::string decode(const std::string &encodedData, std::unordered_map<std::string, char> &reverseCodes) {
    std::string decodedStr, currentCode;
    
    // Longest code length (for optimization)
    size_t maxCodeLength = 0;
    for (const auto& [code, ch] : reverseCodes) {
        maxCodeLength = std::max(maxCodeLength, code.length());
    }
    
    // Skip header if present (first line until newline)
    size_t startPos = 0;
    if (!encodedData.empty() && std::isdigit(encodedData[0])) {
        startPos = encodedData.find('\n');
        if (startPos != std::string::npos) {
            startPos++; // Skip the newline
        } else {
            startPos = 0; // No newline found, start from beginning
        }
    }
    
    std::cout << "Starting decode from position " << startPos << std::endl;
    
    // Process the encoded data
    for (size_t i = startPos; i < encodedData.size(); i++) {
        currentCode += encodedData[i];
        
        // Prevent excessively long codes that won't match
        if (currentCode.length() > maxCodeLength) {
            // Reset if code gets too long and doesn't match anything
            currentCode.clear();
            continue;
        }
        
        if (reverseCodes.find(currentCode) != reverseCodes.end()) {
            decodedStr += reverseCodes[currentCode];
            currentCode.clear();
        }
    }
    
    std::cout << "Decoded " << decodedStr.size() << " bytes from " << encodedData.size() - startPos << " bytes of input" << std::endl;
    return decodedStr;
}