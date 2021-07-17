#include <iostream>
#include <unistd.h>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

const string expected_flags[] = {"-r", "-R", "-L", "-x", "-t"};

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

void parseInput(const int argc, const char* const argv[], map<string, unsigned int> &input_values){
    
    int i = 1;
    while(i < argc) {
        if (find(begin(expected_flags), end(expected_flags), argv[i]) != end(expected_flags)){
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
                exit(1);
            }
        }
        else{
            cout << "unrecognized input flag " << argv[i] << endl;
            printExpectedFlags();
            exit(1);
        }
        i += 2;
    }

    if(checkMissingInputValues(input_values))
        exit(1);
}