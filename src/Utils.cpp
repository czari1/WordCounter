#include "../include/Utils.hpp"
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <locale>
#include <codecvt>

namespace Utils {
    bool fileExists(const std::string& filePath) {
        return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    }

    std::string cleanWord(const std::string& word) {
        std::string result = word;

        result.erase(std::remove_if(result.begin(), result.end(), 
            [](unsigned char c) { return !std::isalpha(c); }), result.end());
        
        std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });

        return result;
    }

    bool isTextFile(const std::string& filePath) {
        std::filesystem::path path(filePath);
        std::string extension = path.extension().string();

        std::vector<std::string> textExtensions = {
            ".txt", ".log", ".csv", ".md", ".xml", ".json",".cpp", ".h",
            ".hpp", ".py", ".java", ".c", ".js", ".html", ".css",
            ".php", ".rb", ".go", ".swift", ".ts", ".vb", ".pl",
            ".sql", ".yaml", ".yml"
        };

        for (const auto& ext : textExtensions) {
        
            if (extension == ext) {
                return true;
            }
        }
        return false;
    }
    
    std::vector<std::string> tokenizeText(const std::string& text) {
        std::vector<std::string> tokens;
        std::istringstream iss(text);
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::unordered_map<char, std::size_t> countLetters(const std::string& text) {
        std::unordered_map<char, std::size_t> letterCount;

        for (char c :text) {

            if (isLetter(c)) {
                letterCount[std::tolower(c)]++;
            }
        };
        return letterCount;
    }

    bool isLetter(unsigned char c) {

        if (std::isalpha(c)) {
            return true;
        }

        static const std::string polishLetters = "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ";
        return polishLetters.find(c) != std::string::npos;
    }
}