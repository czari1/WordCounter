#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include "../include/WordCounter.hpp"
#include "../include/Utils.hpp"
#include "../include/Exceptions.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsoleForUnicode() {
#ifdef _WIN32
    // Simple UTF-8 setup for Windows console
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void showUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " <file1> <file2> ... <fileN>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help       Show this help message" << std::endl;
    std::cout << "  -t, --threads    Number of threads to use (default: auto-detect)" << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  " << programName << " file1.txt file2.txt" << std::endl;
    std::cout << "  " << programName << " -t 4 file1.txt file2.txt" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        // Setup console for Unicode support
        setupConsoleForUnicode();
        
        std::vector<std::string> filePaths;
        std::size_t threadCount = 0; 

        // Parse command line arguments
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
                        std::cerr << "Error: Invalid thread count argument." << std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "Error: -t option requires a number." << std::endl;
                    return 1;
                }
            } else {
                filePaths.push_back(arg);
            }        
        }

        // Interactive file input if no files provided
        if (filePaths.empty()) {
            std::cout << "Provide a path for files (empty line to finish):" << std::endl;
            std::string path;

            while (true) {
                std::cout << "> ";
                if (!std::getline(std::cin, path)) {
                    break; // EOF or error
                }

                if (path.empty()) {
                    break;
                }

                if (Utils::fileExists(path)) {
                    filePaths.push_back(path);
                    std::cout << "Added: " << path << std::endl;
                } else {
                    std::cerr << "File does not exist: " << path << std::endl;
                }
            }
        }

        // Interactive thread count input if not specified
        if (threadCount == 0 && filePaths.empty() == false) {
            std::cout << "Give number of threads (0 for auto-detect): ";
            std::string input;
            if (std::getline(std::cin, input) && !input.empty()) {
                try {
                    threadCount = std::stoul(input);
                } catch (const std::exception&) {
                    std::cerr << "Warning: Invalid thread count. Using auto-detect." << std::endl;
                    threadCount = 0;
                }
            }
        }

        // Check if we have files to process
        if (filePaths.empty()) {
            std::cerr << "No files provided for processing." << std::endl;
            showUsage(argv[0]);
            return 1;
        }

        // Process files
        std::cout << "Processing " << filePaths.size() << " files..." << std::endl;
        WordCounter counter(threadCount);
        counter.processFiles(filePaths);
        counter.printSummary();

        // Get output file name
        std::string outputFile = "wordCountSummary.txt";
        std::cout << "Provide output file name (default: " << outputFile << "): ";
        std::string userOutput;
        if (std::getline(std::cin, userOutput) && !userOutput.empty()) {
            outputFile = userOutput;
        }

        counter.saveResultToFile(outputFile);
        
    } catch (const Exceptions::WordCounterException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}