#pragma once
#include <string>
#include <chrono>
#include <unordered_map>

struct WordCountStats {
    std::string fileName;
    std::size_t totalWords;
    std::size_t uniqueWords;
    std::chrono::milliseconds processingTime;
};