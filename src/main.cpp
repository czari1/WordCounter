#include <iostream>
#include <vector>
#include <string>
#include "../include/WordCounter.hpp"
#include "../include/Utils.hpp"
#include "../include/Exceptions.hpp"

void showUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " <file1> <file2> ... <fileN>\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help       Show this help message\n";
    std::cout << "  -t, --threads    Number of threads to use (default: auto-detect)\n";
    std::cout << " Example:\n";
    std::cout << "  " << programName << " file1.txt file2.txt\n";
    std::cout << "  " << programName << " -t 4 file1.txt file2.txt\n";
}

int main(int argc, char* argv[]) {
    std::vector<std::string> filePaths;
    std::size_t threadCount = 0; 

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            showUsage(argv[0]);
            return 0;
        } else if (arg == "-t" || arg == "--threads") {
            if (i + 1 < argc) {
                try {
                    threadCount = std::stoul(argv[++i]);
                } catch (const std::invalid_argument&) {
                    throw Exceptions::WordCounterException("Invalid thread count argument.");
                    return 1;
                }
            }          
            
        } else {
            filePaths.push_back(arg);
        }        
    }

    if (filePaths.empty()) {
        std::cout << "Provide a path for files (empty to finish): \n";
        std::string path;

        while (true) {
            std::cout << "> ";
            std::getline(std::cin, path);

            if (path.empty()) {
                break;
            }

            if(Utils::fileExists(path)) {
                filePaths.push_back(path);
            } else {
                std::cerr << "File does not exist: " << path << std::endl;
            }
        }
    }

    if (threadCount == 0) {
        std::cout << "Give number of threads (0 for auto-detect): ";
        std::string input;
        std::getline(std::cin, input);

        if (!input.empty()) {
            
            try {
                threadCount = std::stoul(input);
            } catch (const std::exception& e) {
                throw Exceptions::ThreadException("Invalid thread count argument.");
            }
        }
    }

    if (filePaths.empty()) {
        std::cerr << "No files provided for processing." << std::endl;
        showUsage(argv[0]);
        return 1;
    }

    std::cout << "Processing " << filePaths.size() << " files"  << std::endl;
    WordCounter counter(threadCount);
    counter.processFiles(filePaths);
    counter.printSummary();

    std::string outputFile = "wordCountSummary.txt";
    std::cout << "Provide output file name (default: " << outputFile << "): ";
    std::string userOutput;
    std::getline(std::cin, userOutput);

    if (!userOutput.empty()) {
        outputFile = userOutput;
    }

    counter.saveResultToFile(outputFile);

    return 0;
}