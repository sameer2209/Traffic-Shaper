#ifndef REQUEST_TOKEN_H
#define REQUEST_TOKEN_H

#include <queue>

void* startTokenThread(void* inputData);

class TokenBuffer{
    public:
        std::queue<int> tokenBuf;
        int maxBufferSize;
        void setMaxBufferSize(int size);
        void push(int e);
        int size();
        void pop();
};

#endif