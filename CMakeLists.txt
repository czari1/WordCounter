cmake_minimum_required(VERSION 3.10)
project(WordCounter VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Utils.cpp
    src/WordCounter.cpp
    src/WordProcessor.cpp
)

# Header files
set(HEADERS
    include/Exceptions.hpp
    include/Stats.hpp
    include/Utils.hpp
    include/WordCounter.hpp
    include/WordProcessor.hpp
)

# Create executable
add_executable(word_counter ${SOURCES} ${HEADERS})

# Add compiler warnings
if(MSVC)
    target_compile_options(word_counter PRIVATE /W4)
else()
    target_compile_options(word_counter PRIVATE -Wall -Wextra -pedantic)
endif()

# Add threading support
find_package(Threads REQUIRED)
target_link_libraries(word_counter PRIVATE Threads::Threads)

# Install target
install(TARGETS word_counter DESTINATION bin)

# Testing (optional)
option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)