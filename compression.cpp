#include "compression.h"

void compressImage(const std::string &imagePath, const std::string &outputPath) {
    std::cout << "Compressing: " << imagePath << " -> " << outputPath << std::endl;
    
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Error loading image!" << std::endl;
        return;
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

    // Save compressed image as JPEG
    std::vector<int> compressionParams = {cv::IMWRITE_JPEG_QUALITY, 50}; // Adjust quality
    std::string compressedImagePath = outputPath + "_compressed" + ".jpg";
    if (!cv::imwrite(compressedImagePath, img, compressionParams)) {
        std::cerr << "Error saving compressed image!" << std::endl;
    } else {
        std::cout << "Compressed image saved at: " << compressedImagePath << std::endl;
    }
}
