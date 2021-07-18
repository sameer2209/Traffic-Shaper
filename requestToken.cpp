#include "requestToken.h"
#include "main.h"
#include <queue>
#include <iostream>
#include <unistd.h>

using namespace std;

pthread_mutex_t lockTokenBuffer;

class TokenBuffer{
    public:
        queue<int> q2;
        int maxBufferSize;

        TokenBuffer(int buffferSize){
            maxBufferSize = buffferSize;
        }

        void push(int ele){
            if(q2.size() < maxBufferSize){
                q2.push(ele);
                cout << "new token enters the token buffer" << endl;
            }
            else{
                cout << "token buffer full, dropping token" << endl;
            }
        }
};

void* startTokenThread(void* inputData){
    int tokenRate = ((InputData*) inputData)->tokenRate;
    int bufferSize = ((InputData*) inputData)->tokenBufferSize;
    pthread_mutex_init(&lockTokenBuffer, 0);
    TokenBuffer buffer(bufferSize);
    while(true){
        sleep(tokenRate);
        cout << "new token arrives" << endl;
        pthread_mutex_lock(&lockTokenBuffer);
        buffer.push(1);
        pthread_mutex_unlock(&lockTokenBuffer);
    }
}