#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <opencv2/opencv.hpp>
#include "huffman.h"

void decompressImage(const std::string &compressedPath, const std::string &outputPath, int width, int height);

#endif