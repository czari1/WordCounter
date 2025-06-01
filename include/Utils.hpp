#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Utils {
    bool fileExists(const std::string& filePath);
    std::string cleanWord(const std::string& word);
    bool isTextFIle(const std::string& filepath);
    std::vector<std::string> tokenizeText(const std::string& text);
    bool isLetter(unsigned char c);
}