#include "compression.h"

using namespace std;

void compressImage(const string &imagePath, const string &outputPath, 
                   const vector<int>& compressionParams) {
    cout << "Compressing: " << imagePath << " -> " << outputPath << endl;
    
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        throw runtime_error("Error loading image!");
    }

    // Converting pixel data to string
    string pixelData(img.begin<unsigned char>(), img.end<unsigned char>());

    // Huffman Encoding
    unordered_map<char, string> codes;
    unordered_map<string, char> reverseCodes;
    Node* root = nullptr;
    buildHuffmanTree(pixelData, codes, reverseCodes, root);

    // Save Huffman Codes
    saveHuffmanCodes(outputPath + ".bin" + ".codes", codes);

    // Encode data
    string encodedData = encode(pixelData, codes);

    // Save compressed binary file
    ofstream outFile(outputPath + ".bin", ios::binary);
    outFile << img.cols << " " << img.rows << "\n" << encodedData;
    outFile.close();

    // Save compressed image as JPEG with variable quality
    string compressedImagePath = outputPath + "_compressed" + ".jpg";
    if (!cv::imwrite(compressedImagePath, img, compressionParams)) {
        throw runtime_error("Error saving compressed image!");
    }

    cout << "Compressed image saved at: " << compressedImagePath << endl;
}
