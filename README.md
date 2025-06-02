# WordCounter

A high-performance, multi-threaded C++ application for analyzing text files. WordCounter provides detailed statistics including word frequency, letter frequency, and processing performance metrics.

## Features

- **Multi-threaded Processing**: Parallel processing using configurable thread count
- **Word Frequency Analysis**: Count and rank the most frequent words
- **Letter Frequency Analysis**: Analyze character distribution in text
- **UTF-8 Support**: Handle international characters and UTF-8 encoded files
- **Multiple File Processing**: Process multiple text files simultaneously
- **Performance Metrics**: Track processing time for each file
- **Export Results**: Save analysis results to output files

## Requirements

- **C++17** compatible compiler
- **CMake 3.10** or higher
- **Threading support** (pthread/std::thread)
- **Windows/Linux/macOS** support

## Build Instructions

### Windows (MinGW)

```bash
mkdir build
cd build
cmake ..
mingw32-make
```

### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build .
```

### Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Basic Usage

```bash
./word_counter
```

The application will prompt you for:

1. **File paths**: Enter paths to text files (one per line, empty line to finish)
2. **Thread count**: Number of threads to use (0 for auto-detection)
3. **Output file**: Optional file to save results

### Example Session

```
Provide a path for files (empty to finish):
> sample.txt
> document.txt
>
Give number of threads (0 for auto-detect): 4
Processing 2 files
Using 4 threads for processing.

Word Count Summary
===================
Total Unique Words: 150
Total Files Processed: 2
Total Processing Time: 45 ms

5 most frequent words:
the            : 25
and            : 18
to             : 15
of             : 12
in             : 10

5 most frequent letters:
e    : 120
t    : 95
a    : 85
o    : 78
i    : 65
```

## Project Structure

```
WordCounter/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── .gitignore             # Git ignore rules
├── include/               # Header files
│   ├── WordCounter.hpp    # Main word counter class
│   ├── WordProcessor.hpp  # Text processing utilities
│   ├── Utils.hpp          # Utility functions
│   ├── Stats.hpp          # Statistics structures
│   └── Exceptions.hpp     # Custom exceptions
├── src/                   # Source files
│   ├── main.cpp          # Application entry point
│   ├── WordCounter.cpp   # Main implementation
│   ├── WordProcessor.cpp # Text processing implementation
│   └── Utils.cpp         # Utility implementations
└── build/                 # Build directory (generated)
```

## Key Classes and Components

### WordCounter

- Main orchestrator class
- Manages multi-threaded file processing
- Aggregates statistics from all files
- Provides summary and export functionality

### WordProcessor

- Core text processing engine
- File reading with UTF-8 support
- Word tokenization and cleaning
- Word and letter frequency counting

### Utils

- File validation and detection
- Text tokenization utilities
- UTF-8 and encoding support
- Character classification

## Performance Features

- **Parallel Processing**: Utilizes multiple CPU cores for faster processing
- **Memory Efficient**: Streams large files without loading entirely into memory
- **UTF-8 Optimized**: Efficient handling of international character sets
- **Scalable**: Performance scales with available CPU cores

## Supported File Formats

- Plain text files (.txt)
- UTF-8 encoded files (with or without BOM)
- Any text-based file format

## Output Options

- **Console Output**: Real-time progress and summary statistics
- **File Export**: Save detailed results to text files
- **JSON Export**: _(Future feature)_

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Development

### Building in Debug Mode

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Running Tests (if implemented)

```bash
cmake -DBUILD_TESTS=ON ..
make
ctest
```

## Troubleshooting

### Common Issues

**Locale Errors**:

```
Solution: The application uses C locale by default for maximum compatibility
```

**File Not Found**:

```
Solution: Ensure file paths are correct and files are accessible
```

**Threading Issues**:

```
Solution: Try setting thread count to 1 for debugging
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Created for efficient text analysis and word counting applications.

## Version History

- **v1.0**: Initial release with basic word counting
- **v1.1**: Added multi-threading support
- **v1.2**: UTF-8 support and letter frequency analysis
