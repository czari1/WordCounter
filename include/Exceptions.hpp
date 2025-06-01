#pragma once
#include <stdexcept>
#include <string>

namespace Exceptions {

    class WordCounterException : public std::runtime_error {
    public:
        explicit WordCounterException(const std::string& message)
            : std::runtime_error("WordCounter Error: " + message) {}
    };

    class FileException : public WordCounterException {
    public:
        explicit FileException(const std::string& fileName, const std::string& message)
            : WordCounterException("File error: " + message + "[File: " + fileName + "]"),
            m_fileName(fileName) {}
        
            const std::string& getFileName() const {
                return m_fileName;
            }
    private:
        std::string m_fileName;
    };

    class FileNotFoundException : public FileException {
    public:
    explicit FileNotFoundException(const std::string& fileName)
        : FileException(fileName, "File not found.") {}
    };

    class FilePermissionException : public FileException {
    public:
        explicit FilePermissionException(const std::string& fileName)
            : FileException(fileName, "Permission denied.") {}
    };

    class FileProcessingException : public FileException {
    public:
        explicit FileProcessingException(const std::string& fileName, const std::string& reason) 
            : FileException(fileName, "Processing error: " + reason) {}
    };

    class InvalidFileFormatException : public FileException {
    public:
        explicit InvalidFileFormatException(const std::string& fileName, const std::string& format)
            : FileException(fileName, "Invalid file format: " + format) {}
    };

    class MemoryException : public WordCounterException {
    public:
        explicit MemoryException(const std::string& operation)
            : WordCounterException("Memory allocation error during: " + operation) {}
    };

    class ThreadException : public WordCounterException {
    public:
        explicit ThreadException(const std::string& message)
            : WordCounterException("Thread error: " + message) {}
    };

    class OutputException : public WordCounterException {
    public:
        explicit OutputException(const std::string& fileName, const std::string& message)
            : WordCounterException("Output error: " + message + "[File: " + fileName + "]"),
            m_fileName(fileName) {}
        const std::string& getFileName() const {
            return m_fileName;
        }
    private:
        std::string m_fileName;
    };

    class InvalidArgumentException : public WordCounterException {
    public:
        explicit InvalidArgumentException(const std::string& argument, const std::string& reason)
            : WordCounterException("Invalid argument: " + argument + ". Reason: " + reason) {}
    };
}