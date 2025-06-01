#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Utils {
    bool fileExists(const std::string& filePath);
    std::string cleanWord(const std::string& word);
    bool isTextFile(const std::string& filepath);
    std::vector<std::string> tokenizeText(const std::string& text);
    bool isLetter(unsigned char c);
    bool isOtherLetter(wchar_t wc);
    bool isUTF8File(const std::string& filePath);
}