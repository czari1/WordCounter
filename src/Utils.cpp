#include "../include/Utils.hpp"
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <locale>
#include <codecvt>
#include <fstream>

namespace Utils {
    bool fileExists(const std::string& filePath) {
        return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    }

    std::string cleanWord(const std::string& word) {
        std::string result = word;
        result.reserve(word.length());

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wword;

        try {
            wword = converter.from_bytes(result);
        } catch (const std::range_error&) {

            for (unsigned char c : word) {

                if (std::isalpha(c)) {
                    result += std::tolower(c);
                }
            }
            
            return result;
        }

        std::wstring cleaned;
        cleaned.reserve(wword.length());

        for (wchar_t wc : wword ) {

            if (std::iswalpha(wc)) {
                cleaned += std::tolower(wc);
            }
        }

        try {
            result = converter.to_bytes(cleaned);
        } catch (const std::range_error&) {
            result.clear();
        }
        
        return result;
    }

    bool isTextFile(const std::string& filePath) {
        std::filesystem::path path(filePath);
        std::string extension = path.extension().string();

        std::transform(extension.begin(), extension.end(), extension.begin(),
                      [](unsigned char c) { return std::tolower(c); });

        std::vector<std::string> textExtensions = {
            ".txt", ".log", ".csv", ".md", ".xml", ".json", ".cpp", ".h",
            ".hpp", ".py", ".java", ".c", ".js", ".html", ".css",
            ".php", ".rb", ".go", ".swift", ".ts", ".vb", ".pl",
            ".sql", ".yaml", ".yml", ".rst", ".tex", ".rtf"
        };

        return std::find(textExtensions.begin(), textExtensions.end(), extension) != textExtensions.end();
    }
    
    std::vector<std::string> tokenizeText(const std::string& text) {
        std::vector<std::string> tokens;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wtext;

        try {
            wtext = converter.from_bytes(text);
        } catch (const std::range_error&) {
            std::istringstream iss(text);
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            return tokens;
        }

        std::wstring currentToken;

        for (wchar_t wc : wtext ) {

            if (std::iswspace(wc) || std::iswpunct(wc)) {

                if (!currentToken.empty()) {

                    try {
                        tokens.push_back(converter.to_bytes(currentToken));
                    } catch (const std::range_error&) {

                    }
                    currentToken.clear();
                } 
            } else {
                currentToken += wc;
            }
        }

        if (!currentToken.empty()) {

            try {
                tokens.push_back(converter.to_bytes(currentToken));
            } catch (const std::range_error&) {

            }
        }

        return tokens;
    }


    bool isLetter(unsigned char c) {
        
        return std::isalpha(c);
    }

    bool isOtherLetter(wchar_t wc) {
        return std::iswalpha(wc);
    }

    bool isUTF8File(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        
        if (!file) {
            
            return false;
        }
        
        char bom[3];
        file.read(bom,3);

        if (file.gcount() == 3 && 
            static_cast<unsigned char>(bom[0]) == 0xEF &&
            static_cast<unsigned char>(bom[1]) == 0xBB &&
            static_cast<unsigned char>(bom[2]) == 0xBF) {
            return true;
        }
        
        file.seekg(0);
        std::vector<char> buffer(1024);
        file.read(buffer.data(), buffer.size());
        std::size_t bytesRead = file.gcount();
        
        std::size_t i;

        while (i < bytesRead) {
            unsigned char byte = static_cast <unsigned char> (buffer[i]);
            
            if (byte <= 0x7F) {
                
                i++;
            
            } else if ((byte >> 5) == 0x06) {
                
                if (i + 1 >= bytesRead) break;
                
                if ((static_cast<unsigned char>(buffer[i + 1]) >> 6) != 0x02) return false;
                i += 2;
            
            } else if ((byte >> 4) == 0x0E) {
            
                if (i + 2 >= bytesRead) break;
                
                if ((static_cast<unsigned char>(buffer[i + 1]) >> 6) != 0x02 ||
                    (static_cast<unsigned char>(buffer[i + 2]) >> 6) != 0x02) return false;
                i += 3;
            
            } else if ((byte >> 3) == 0x1E) {

                if (i + 3 >= bytesRead) break;
                
                if ((static_cast<unsigned char>(buffer[i + 1]) >> 6) != 0x02 ||
                    (static_cast<unsigned char>(buffer[i + 2]) >> 6) != 0x02 ||
                    (static_cast<unsigned char>(buffer[i + 3]) >> 6) != 0x02) return false;
                i += 4;
            
            } else {
                
                return false;
            } 
        }

        return true;
    }
}