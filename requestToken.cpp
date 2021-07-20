#include "requestToken.h"
#include "main.h"
#include "logger.h"
#include <iostream>
#include <unistd.h>

using namespace std;

pthread_mutex_t lockTokenBuffer;
pthread_cond_t condTokenBuffer;

int TokenBuffer::tokenCount = 0;

void TokenBuffer::setMaxBufferSize(int size){
    maxBufferSize = size;
}

void TokenBuffer::push(int ele, int logLevel){
    if(tokenBuf.size() < maxBufferSize){
        tokenBuf.push(ele);
        if(logLevel == 2){
            // cout << "token number " << ele << " enters the token buffer" << endl;
            string msg = "token number " + to_string(ele) + " enters the token buffer";
            printLog(msg);
        }
            
    }
    else{
        if(logLevel == 2){
            // cout << "token buffer full, dropping token number " << ele << endl;
            string msg = "token buffer full, dropping token number " + to_string(ele);
            printLog(msg);
        }
    }
}

int TokenBuffer::size(){
    return tokenBuf.size();
}

void TokenBuffer::pop(){
    tokenBuf.pop();
}

int TokenBuffer::front(){
    return tokenBuf.front();
}

TokenBuffer buffer;

void* startTokenThread(void* inputData){
    int tokenRate = ((InputData*) inputData)->tokenRate;
    int bufferSize = ((InputData*) inputData)->tokenBufferSize;
    int logLevel = ((InputData*) inputData)->logLevel;
    buffer.setMaxBufferSize(bufferSize);
    pthread_mutex_init(&lockTokenBuffer, 0);
    pthread_cond_init(&condTokenBuffer, 0);
    while(true){
        sleep(tokenRate);
        TokenBuffer::tokenCount++;
        pthread_mutex_lock(&lockTokenBuffer);
        if(logLevel == 2){
            cout << "token number " << TokenBuffer::tokenCount << " arrives" << endl;
            string msg = "token number " + to_string(TokenBuffer::tokenCount) + " arrives";
            printLog(msg);
        }
        buffer.push(TokenBuffer::tokenCount, logLevel);
        pthread_cond_signal(&condTokenBuffer);
        pthread_mutex_unlock(&lockTokenBuffer);
    }
}