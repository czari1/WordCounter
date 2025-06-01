#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <chrono>
#include "Stats.hpp"

class WordCounter {
public:
    explicit WordCounter(std::size_t numThreads = 0);

    void processFiles(const std::vector<std::string>& filePaths);
    void saveResultToFile(const std::string& outputFilePath) const;
    void printSummary() const;
    std::size_t getThreadCount() const;
    std::size_t getTotalUniqueWords() const;
    std::chrono::milliseconds getTotalProcessingTime() const;
private:
    std::size_t m_threadCount;
    std::mutex m_mutex;
    std::unordered_map<std::string, std::size_t> m_globalWordCount;
    std::vector<WordCountStats> m_fileStats;
    std::chrono::milliseconds m_totalProcessingTime{0};
};