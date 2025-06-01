#include "../include/WordProcessor.hpp"
#include "../include/Exceptions.hpp"
#include "../include/Utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

WordCountStats WordProcessor::processFile(const std::string& filePath) {
    WordCountStats stats;
    stats.fileName = filePath;

    auto startTIme = std::chrono::high_resolution_clock::now();

    try {
        std::string fileContents = readFileContents(filePath);
        auto wordCount = countWords(fileContents);
        stats.uniqueWords = wordCount.size();

        stats.totalWords=0;
        
        for (const auto& [word, count] : wordCount)  {
            stats.totalWords += count;
        }

    
    } catch (const std::exception& e) {
        throw Exceptions::FileProcessingException(filePath, e.what());
    }

    auto endTIme = std::chrono::high_resolution_clock::now();
    stats.processingTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTIme - startTIme);

    return stats;
}

std::string WordProcessor::readFileContents(const std::string& filePath) {
    std::ifstream file(filePath);
    
    if (!file) {
        throw Exceptions::FileNotFoundException(filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::unordered_map<std::string, std::size_t> WordProcessor::countWords(const std::string& text) {
    std::unordered_map<std::string, std::size_t> wordCount;
    auto tokens = Utils::tokenizeText(text);

    for (const auto& token : tokens) {
        std::string cleanedWord = Utils::cleanWord(token);
        
        if (!cleanedWord.empty()) {
            wordCount[cleanedWord]++;
        } 
    }

    return wordCount;
}

