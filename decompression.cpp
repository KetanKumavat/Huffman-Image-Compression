#include "decompression.h"
#include <iostream>
#include <fstream>
#include <vector>

void decompressImage(const std::string &compressedPath, const std::string &outputPath, int width, int height) {
    // Open the compressed file
    std::ifstream inFile(compressedPath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Could not open compressed file: " << compressedPath << std::endl;
        return;
    }
    
    // Read the entire file content
    std::string encodedData((std::istreambuf_iterator<char>(inFile)),
                           std::istreambuf_iterator<char>());
    inFile.close();
    
    // Check if we have data
    if (encodedData.empty()) {
        std::cerr << "Error: Compressed file is empty" << std::endl;
        return;
    }
    
    // Load Huffman codes
    std::string codesPath = compressedPath + ".codes";
    std::unordered_map<char, std::string> codes;
    std::unordered_map<std::string, char> reverseCodes;
    
    if (!loadHuffmanCodes(codesPath, codes, reverseCodes)) {
        std::cerr << "Error: Could not load Huffman codes from: " << codesPath << std::endl;
        return;
    }
    
    // Decode the data
    std::string decodedData = decode(encodedData, reverseCodes);
    
    // Check if we have enough decoded data
    size_t requiredSize = width * height;
    if (decodedData.size() < requiredSize) {
        std::cerr << "Error: Decoded data size (" << decodedData.size() 
                  << ") is smaller than required size (" << requiredSize << ")" << std::endl;
        return;
    }
    
    // Create image with proper size and type
    cv::Mat img(height, width, CV_8UC1);
    std::memcpy(img.data, decodedData.data(), requiredSize);
    
    // Write the image
    if (!cv::imwrite(outputPath, img)) {
        std::cerr << "Error: Could not write output image: " << outputPath << std::endl;
        return;
    }
    
    std::cout << "Decompression successful: " << outputPath << std::endl;
}