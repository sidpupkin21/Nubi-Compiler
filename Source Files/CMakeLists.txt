# CMakeList.txt : CMake project for NUBI, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("NUBI")

# Add source to this project's executable.
add_executable (NUBI "Scanner.c" "Header Files/Scanner.h"
"Parser.c" "Header Files/Parser.h" 
"Header Files/Buffer.h" "Buffer.c" 
"Header Files/Compilers.h" "Compilers.c" 
"MainScanner.c" "MainParser.c" "MainBuffer.c")

# TODO: Add tests and install targets if needed.
