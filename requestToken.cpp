#include "requestToken.h"
#include "trafficShaper.h"
#include <iostream>
#include <unistd.h>

using namespace std;

// mutex lock for the token buffer
pthread_mutex_t lockTokenBuffer;

// condition variable for the token buffer
pthread_cond_t condTokenBuffer;

extern pthread_mutex_t lockStdOut;

// static variable to store the created token counts
int TokenBuffer::tokenCount = 0;

// function to fix the token buffer size
void TokenBuffer::setMaxBufferSize(int size){
    maxBufferSize = size;
}

// function to add created tokens to the token buffer
void TokenBuffer::push(int ele, int logLevel){
    if(tokenBuf.size() < maxBufferSize){
        tokenBuf.push(ele);
        if(logLevel == 2)
            cout << "token number " << ele << " enters the token buffer" << endl;
            
    }
    else{
        if(logLevel == 2)
            cout << "token buffer full, dropping token number " << ele << endl;
    }
}

// function to get the current count of tokens in the token buffer 
int TokenBuffer::size(){
    return tokenBuf.size();
}

// function to remove the tokens from the token buffer
void TokenBuffer::pop(){
    tokenBuf.pop();
}

// function to get the front element in the token buffer
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
        // acquire the mutex lock for the token buffer
        pthread_mutex_lock(&lockTokenBuffer);
        if(logLevel == 2){
            pthread_mutex_lock(&lockStdOut);
            cout << "token number " << TokenBuffer::tokenCount << " arrives" << endl;
            pthread_mutex_unlock(&lockStdOut);
        }
        
        // add the new tokens to the token buffer
        buffer.push(TokenBuffer::tokenCount, logLevel);
        // signal the main thread
        pthread_cond_signal(&condTokenBuffer);
        // release the mutex lock for the token buffer
        pthread_mutex_unlock(&lockTokenBuffer);
    }
}