#include "requestToken.h"
#include "main.h"
#include <iostream>
#include <unistd.h>

using namespace std;

pthread_mutex_t lockTokenBuffer;
pthread_cond_t condTokenBuffer;

void TokenBuffer::setMaxBufferSize(int size){
    maxBufferSize = size;
}

void TokenBuffer::push(int ele){
    if(tokenBuf.size() < maxBufferSize){
        tokenBuf.push(ele);
        std::cout << "new token enters the token buffer" << std::endl;
    }
    else{
        std::cout << "token buffer full, dropping token" << std::endl;
    }
}

int TokenBuffer::size(){
    return tokenBuf.size();
}

void TokenBuffer::pop(){
    tokenBuf.pop();
}

TokenBuffer buffer;

void* startTokenThread(void* inputData){
    int tokenRate = ((InputData*) inputData)->tokenRate;
    int bufferSize = ((InputData*) inputData)->tokenBufferSize;
    buffer.setMaxBufferSize(bufferSize);
    pthread_mutex_init(&lockTokenBuffer, 0);
    pthread_cond_init(&condTokenBuffer, 0);
    while(true){
        sleep(tokenRate);
        pthread_mutex_lock(&lockTokenBuffer);
        cout << "new token arrives" << endl;
        buffer.push(1);
        pthread_cond_signal(&condTokenBuffer);
        pthread_mutex_unlock(&lockTokenBuffer);
    }
}