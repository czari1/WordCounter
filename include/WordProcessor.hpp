#pragma once
#include <string>
#include <unordered_map>
#include "Stats.hpp"

class WordProcessor {
public:
    static WordCountStats processFile(const std::string& filePath);

private:
    static std::string readFileContents(const std::string& filePath);
    static std::unordered_map<std::string, std::size_t> countWords(const std::string& text);
};