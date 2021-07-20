#ifndef REQUEST_TOKEN_H
#define REQUEST_TOKEN_H

#include <queue>

void* startTokenThread(void* inputData);

class TokenBuffer{
    public:
        static int tokenCount;
        std::queue<int> tokenBuf;
        int maxBufferSize;
        void setMaxBufferSize(int size);
        void push(int e, int logLevel);
        int size();
        void pop();
        int front();
};

#endif