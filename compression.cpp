#include "compression.h"

void compressImage(const std::string &imagePath, const std::string &outputPath, 
                   const std::vector<int>& compressionParams) {
    std::cout << "Compressing: " << imagePath << " -> " << outputPath << std::endl;
    
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        throw std::runtime_error("Error loading image!");
    }

    // Convert pixel data to string
    std::string pixelData(img.begin<unsigned char>(), img.end<unsigned char>());

    // Huffman Encoding
    std::unordered_map<char, std::string> codes;
    std::unordered_map<std::string, char> reverseCodes;
    Node* root = nullptr;
    buildHuffmanTree(pixelData, codes, reverseCodes, root);

    // Save Huffman Codes
    saveHuffmanCodes(outputPath + ".bin" + ".codes", codes);

    // Encode data
    std::string encodedData = encode(pixelData, codes);

    // Save compressed binary file
    std::ofstream outFile(outputPath + ".bin", std::ios::binary);
    outFile << img.cols << " " << img.rows << "\n" << encodedData;
    outFile.close();

    // Save compressed image as JPEG with variable quality
    std::string compressedImagePath = outputPath + "_compressed" + ".jpg";
    if (!cv::imwrite(compressedImagePath, img, compressionParams)) {
        throw std::runtime_error("Error saving compressed image!");
    }

    std::cout << "Compressed image saved at: " << compressedImagePath << std::endl;
}
