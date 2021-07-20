#include "inputParser.h"
#include "main.h"
#include "request.h"
#include "requestServer.h"
#include "requestToken.h"
#include "logger.h"
#include <iostream>
#include <pthread.h>
#include <queue>

using namespace std;

map<string, unsigned int> input_values;
InputData inputData;

extern queue<Request*> q1;
extern pthread_mutex_t lockQ1;
extern pthread_cond_t condQ1;

extern pthread_mutex_t lockTokenBuffer;
extern TokenBuffer buffer;
extern pthread_cond_t condTokenBuffer;

extern queue<Request*> q2;
extern pthread_mutex_t lockQ2;
extern pthread_cond_t condQ2;

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

    // cout << "emulation begin" << endl;
    printLog("emulation begin");
    pthread_t request_thread, token_thread, server_thread;
    
    int req, tok, ser;
    req = pthread_create(&request_thread, NULL, startRequestThread, &inputData);
    tok = pthread_create(&token_thread, NULL, startTokenThread, &inputData);
    ser = pthread_create(&server_thread, NULL, startServerThread, &inputData);
    
    Request* r;
    while(true){
        pthread_mutex_lock(&lockQ1);
        while(q1.empty()){
            pthread_cond_wait(&condQ1, &lockQ1);
        }
        
        pthread_mutex_lock(&lockTokenBuffer);
        while(buffer.size() < inputData.tokenReq){
            pthread_cond_wait(&condTokenBuffer, &lockTokenBuffer);
        }
        
        r = q1.front();
        q1.pop();
        r->setQ1ExitTime();
        int i = 1;
        while(i++ <= inputData.tokenReq){
            if(inputData.logLevel == 2){
                int tokId = buffer.front();
                // cout << "token number " << tokId << " being consumed by request r" << r->getRequestId() << endl;
                string msg = "token number " + to_string(tokId) + " being consumed by request r" + to_string(r->getRequestId());
                printLog(msg);
            }
            buffer.pop();
        }
        // cout << "r" << r->getRequestId() << " leaves Q1, time  in Q1 = " << r->getTimeInQ1() << "ms, remaining token = " << buffer.size() << endl;
        string msg = "r" + to_string(r->getRequestId()) + " leaves Q1, time  in Q1 = " + to_string(r->getTimeInQ1()) + "ms, remaining token = " + to_string(buffer.size());
        printLog(msg);
        
        pthread_mutex_unlock(&lockTokenBuffer);
        pthread_mutex_unlock(&lockQ1);

        pthread_mutex_lock(&lockQ2);
        q2.push(r);
        r->setQ2EntryTime();
        // cout << "r" << r->getRequestId() << " enters Q2" << endl;
        msg = "r" + to_string(r->getRequestId()) + " enters Q2";
        printLog(msg);
        pthread_cond_signal(&condQ2);
        pthread_mutex_unlock(&lockQ2);
    }
    
    pthread_join(request_thread, NULL);
    pthread_join(token_thread, NULL);
    pthread_join(server_thread, NULL);
    return 0;
}