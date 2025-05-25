#ifndef UNIT_TESTS_HPP
#define UNIT_TESTS_HPP

#include <string>
#include <sstream>

void RunTest(std::string& path, int indexFile, std::stringstream& outputBuffer, std::stringstream& expectedOutputBuffer);
void RunPerformanceTest(std::string& path);

#endif