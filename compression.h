#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <opencv2/opencv.hpp>
#include "huffman.h"

void compressImage(const std::string &imagePath, const std::string &outputPath);

#endif