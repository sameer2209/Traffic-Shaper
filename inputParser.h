#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <map>
#include <string>
#include "trafficShaper.h"

void parseInput(const int argc, const char* const argv[], std::map<std::string, unsigned int> &input_values);

void createInputDataStructure(std::map<std::string, unsigned int> input_values, InputData &InputData);

#endif