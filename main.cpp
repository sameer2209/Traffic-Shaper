#include "inputParser.h"
#include <iostream>
#include <pthread.h>

using namespace std;

map<string, unsigned int> input_values;

void printInputValues(){
    cout << "Printing the input options" << endl;
    for(const auto& value : input_values)
        cout << "option: " << value.first << " value: " << value.second << endl;
}

int main(int argc, char* argv[]){

    parseInput(argc, argv, input_values);
    printInputValues();
    return 0;
}