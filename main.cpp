#include "inputParser.h"
#include "main.h"
#include "request.h"
#include "requestToken.h"
#include <iostream>
#include <pthread.h>

using namespace std;

map<string, unsigned int> input_values;
InputData inputData;

void printInputValues(){
    cout << "Printing the input options" << endl;
    for(const auto& value : input_values)
        cout << "option: " << value.first << " value: " << value.second << endl;
}

void printDataFromStruct(){
    cout << "printing data from the structure" << endl;
    cout << "request rate: " << inputData.requestRate << endl;
    cout << "resource required: " << inputData.tokenReq << endl;
    cout << "token rate: " << inputData.tokenRate << endl;
    cout << "buffer size: " << inputData.tokenBufferSize << endl;
    cout << "request service time: " << inputData.requestServiceTime << endl;
}

int main(int argc, char* argv[]){

    parseInput(argc, argv, input_values);
    createInputDataStructure(input_values, inputData);
    printInputValues();
    printDataFromStruct();

    cout << "emulation begin" << endl;
    pthread_t request_thread, token_thread, server_thread;
    int req, tok, ser;
    req = pthread_create(&request_thread, NULL, startRequestThread, &inputData);
    tok = pthread_create(&token_thread, NULL, startTokenThread, &inputData);
    pthread_join(request_thread, NULL);
    pthread_join(token_thread, NULL);
    return 0;
}