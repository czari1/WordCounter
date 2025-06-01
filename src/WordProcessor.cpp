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

    auto endTime = std::chrono::high_resolution_clock::now();
    stats.processingTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTIme);

    return stats;
}

std::string WordProcessor::readFileContents(const std::string& filePath) {
    bool isUTF8 = Utils::isUTF8File(filePath);
    
    std::ifstream file(filePath, std::ios::binary);
    
    if (!file) {
        throw Exceptions::FileNotFoundException(filePath);
    }

    std::string content;
    
    if (isUTF8) {
        char bom[3];
        file.read(bom, 3);
        
        if (file.gcount() == 3 && 
            static_cast<unsigned char>(bom[0]) == 0xEF &&
            static_cast<unsigned char>(bom[1]) == 0xBB &&
            static_cast<unsigned char>(bom[2]) == 0xBF) {

        } else {

            file.seekg(0);
        }
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();

    return content;
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

