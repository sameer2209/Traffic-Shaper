#include "trafficShaper.h"
#include <iostream>
#include <unistd.h>
#include <map>
#include <string.h>
#include <algorithm>

using namespace std;

const string expected_flags[] = {"-r", "-R", "-L", "-x", "-t", "--log-level"};

// function to print the missing input flags and return the missing flag count
int checkMissingInputValues(const map<string, unsigned int> &input_values){
    int missing_val_count = 0;
    for(const auto& flag : expected_flags){
        if(input_values.find(flag) == input_values.end()){
            missing_val_count++;
            cout << "missing input flag " << flag << endl;
        }
    }
    return missing_val_count;
}

bool isNumber(string str){
    for(char c : str){
        if(!isdigit(c))
            return false;
    }
    return true;
}

void printExpectedFlags(){
    cout << "The expected flags are: ";
    for(string flag : expected_flags){
        cout << flag << " ";
    }
    cout << endl;
}

void createInputDataStructure(map<string, unsigned int> input_values, InputData &inputData){
    for(const auto& val : input_values){
        if(val.first == "-r")
            inputData.requestRate = val.second;
        else if(val.first == "-R")
            inputData.tokenReq = val.second;
        else if(val.first == "-L")
            inputData.tokenBufferSize = val.second;
        else if(val.first == "-x")
            inputData.tokenRate = val.second;
        else if(val.first == "-t")
            inputData.requestServiceTime = val.second;
        else if(val.first == "--log-level")
            inputData.logLevel = val.second;
    }
}

bool isValidLogLevel(string str){
    if(isNumber(str)){
        int t = stoi(str);
        if(t == 1 || t == 2)
            return true;
        else
            return false;
    }
    return false;
}

void parseInput(const int argc, const char* const argv[], map<string, unsigned int> &input_values){
    
    int i = 1;
    while(i < argc) {
        if (find(begin(expected_flags), end(expected_flags), argv[i]) != end(expected_flags)){
            if(i+1 == argc){
                cout << "missing input value for option flag " << argv[i] << endl;
                exit(0);
            }
            else{
                if(strcmp(argv[i], "--log-level") == 0){
                    if(!isValidLogLevel(argv[i+1])){
                        cout << "invalid --log-level flag value. Expected value is either 1 or 2" << endl;
                        exit(0);
                    }
                    else{
                        input_values[argv[i]] = stoi(argv[i+1]);
                    }
                    
                }
                else{
                    if(isNumber(argv[i+1])){
                        int t = stoi(argv[i+1]);
                        if (t > 0)
                            input_values[argv[i]] = t;
                        else{
                            cout << "positive vaule expected for option " << argv[i] << endl;
                        }
                    }
                    else{
                        cout << "integer value expected for option " << argv[i] << endl;
                        exit(0);
                    }
                }
            }
        }
        else{
            cout << "unrecognized input flag " << argv[i] << endl;
            printExpectedFlags();
            exit(0);
        }
        i += 2;
    }

    if(checkMissingInputValues(input_values))
        exit(0);
}