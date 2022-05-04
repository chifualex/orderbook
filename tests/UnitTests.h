#ifndef UNIT_TESTS_HPP
#define UNIT_TESTS_HPP

#include <string>
#include <sstream>

void run_performance_test(std::string& path);
void RunTest(std::string& path, int indexFile, std::stringstream& outputBuffer, std::stringstream& expectedOutputBuffer);

#endif