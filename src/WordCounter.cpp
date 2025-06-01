#include "../include/WordCounter.hpp"
#include "../include/WordProcessor.hpp"
#include "../include/Exceptions.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <map>

WordCounter::WordCounter(std::size_t threadCount) 
    : m_threadCount(threadCount == 0 ? std::thread::hardware_concurrency() : threadCount) {

    if (m_threadCount == 0) {
        m_threadCount = 1; 
    }

    try {
        std::locale::global(std::locale("C"));
        std::cout.imbue(std::locale("C"));
    } catch (const std::runtime_error& e) {
        std::cerr << "Warning: Failed to set locale. Using default locale. Error: " << e.what() << std::endl;
    }

    std::cout << "Using " << m_threadCount << " threads for processing for word processing." << std::endl;
}

void WordCounter::processFiles(const std::vector<std::string>& filePaths) {
    auto startTime = std::chrono::high_resolution_clock::now();

    m_globalWordCount.clear();
    m_fileStats.clear();
    m_fileStats.reserve(filePaths.size());
    
    std::vector<std::future<WordCountStats>> futures;
    futures.reserve(filePaths.size());
    
    for (const auto& filePath : filePaths) {
   
        if (!Utils::fileExists(filePath)) {
            std::cerr << "File does not exist: " << filePath << std::endl;
            continue;
        }
        

        futures.push_back(std::async(std::launch::async, [this, filePath]() {
            WordCountStats stats = WordProcessor::processFile(filePath);
                                    std::ifstream inputFile(filePath);
                                    std::string fileContents((std::istreambuf_iterator<char>(inputFile)), 
                                                              std::istreambuf_iterator<char>());
            std::unordered_map<std::string, size_t> localWordCount;
            std::unordered_map<std::string, size_t> localLetterCount;
            auto tokens = Utils::tokenizeText(fileContents);
            
            for (const auto& token : tokens) {
                std::string cleanedWord = Utils::cleanWord(token);
                std::string cleanedLetter = Utils::cleanWord(token);
                
                if (!cleanedWord.empty()) {
                    localWordCount[cleanedWord]++;
                }

                if (!cleanedLetter.empty()) {
                    localLetterCount[cleanedLetter]++;
                }
            }
            
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                
                for (const auto& [word, count] : localWordCount) {
                    m_globalWordCount[word] += count;
                }

            }
            
            std::cout << "Processed files " << filePath 
                      << " (" << stats.totalWords << " words, "
                      << stats.uniqueWords << " unique) in "
                      << stats.processingTime.count() << " ms\n";
            
            return stats;
        }));
    }
    
    for (auto& future : futures) {
        m_fileStats.push_back(future.get());
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    m_totalProcessingTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
}


void WordCounter::saveResultToFile(const std::string& outputFilePath) const {
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if(!outputFile) {
        throw Exceptions::OutputException(outputFilePath, "Unable to open output file.");
    }

    outputFile << static_cast<char>(0xEF) << static_cast<char>(0xBB) << static_cast<char>(0xBF);

    outputFile << "Word Count Summary\n";
    outputFile << "Total Unique Words: " << getTotalUniqueWords() << "\n";
    outputFile << "Total Files Processed: " << m_fileStats.size() << "\n";
    outputFile << "Total Processing Time: " << getTotalProcessingTime().count() << " ms\n\n";

    outputFile << "File Statistics\n";
    
    for (const auto& stats : m_fileStats) {
    outputFile << "File: " << stats.fileName << "\n";
    outputFile << "Total Words: " << stats.totalWords << "\n";
    outputFile << "Unique Words: " << stats.uniqueWords << "\n";
    outputFile << "Processing Time: " << stats.processingTime.count() << " ms\n\n";
    }

    outputFile << "20 most frequent words\n";
    std::vector<std::pair<std::string, std::size_t>> wordFrequency(m_globalWordCount.begin(), m_globalWordCount.end());
    std::sort(wordFrequency.begin(), wordFrequency.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::size_t count = 0;

    for (const auto& [word,freq] : wordFrequency) {
        outputFile << std::setw(20) << std::left << word << ": " << freq << "\n";
        
        if (++count >= 20) {
            break;
        }
    }
   
    std::cout << "Results saved to " << outputFilePath << std::endl;
}

void WordCounter::printSummary() const {
    std::cout << "Word Count Summary\n";
    std::cout << "===================\n";
    std::cout << "Total Unique Words: " << getTotalUniqueWords() << "\n";
    std::cout << "Total Files Processed: " << m_fileStats.size() << "\n";
    std::cout << "Total Processing Time: " << getTotalProcessingTime().count() << " ms\n\n";

    for (const auto& stats: m_fileStats) {
        std::cout << "File: " << stats.fileName << "\n";
        std::cout << "Total Words: " << stats.totalWords << "\n";
        std::cout << "Unique Words: " << stats.uniqueWords << "\n";
        std::cout << "Processing Time: " << stats.processingTime.count() << " ms\n\n";
    }

    std::cout << "5 most frequent words:\n";
    std::vector<std::pair<std::string, std::size_t>> wordFrequency(m_globalWordCount.begin(), m_globalWordCount.end());
    std::sort(wordFrequency.begin(), wordFrequency.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::size_t count = 0;
    
    for (const auto& [word, freq] : wordFrequency) {
        std::cout << std::setw(15) << std::left << word << ": " << freq << "\n";
        
        if (++count >= 5) break;
    }
}

std::size_t WordCounter::getThreadCount() const {
    return m_threadCount;
}

std::size_t WordCounter::getTotalUniqueWords() const {
    return m_globalWordCount.size();
}


std::chrono::milliseconds WordCounter::getTotalProcessingTime() const {
    return m_totalProcessingTime;
}