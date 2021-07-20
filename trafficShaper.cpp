#include "inputParser.h"
#include "trafficShaper.h"
#include "request.h"
#include "requestServer.h"
#include "requestToken.h"
#include <iostream>
#include <pthread.h>
#include <queue>
#include <csignal>

using namespace std;

// map to store input values corresponding the given option flags
map<string, unsigned int> input_values;

// struct to store input values for the task data
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

// variables to store thread ids for the corresponding threads
pthread_t request_thread, token_thread, server_thread;

// function to trigger on intercepting the interrupt signal
void signalCallbackHandler(int signum){
    
    pthread_cancel(request_thread);
    pthread_cancel(token_thread);
    pthread_cancel(server_thread);

    pthread_join(request_thread, NULL);
    pthread_join(token_thread, NULL);
    pthread_join(server_thread, NULL);

    cout << "emulation ends" << endl;
    exit(signum);
}

int main(int argc, char* argv[]){

    // Register signal and signal handler
    signal(SIGINT, signalCallbackHandler);
    
    parseInput(argc, argv, input_values);
    createInputDataStructure(input_values, inputData);
    
    cout << "emulation begin" << endl;
    
    int req, tok, ser;

    req = pthread_create(&request_thread, NULL, startRequestThread, &inputData);
    tok = pthread_create(&token_thread, NULL, startTokenThread, &inputData);
    ser = pthread_create(&server_thread, NULL, startServerThread, &inputData);

    if(req){
        cout << "error creating the request thread, exiting..." << endl;
        exit(0);
    }

    if(tok){
        cout << "error creating the token thread, exiting..." << endl;
        exit(0);
    }

    if(ser){
        cout << "error creating the server thread, exiting..." << endl;
        exit(0);
    }
    
    Request* r;
    while(true){
        // acquire mutex lock for the Q1 buffer
        pthread_mutex_lock(&lockQ1);
        while(q1.empty()){
            // wait for the Q1 condition variable to signal
            pthread_cond_wait(&condQ1, &lockQ1);
        }
        
        // acquire mutex lock for token buffer
        pthread_mutex_lock(&lockTokenBuffer);
        while(buffer.size() < inputData.tokenReq){
            // wait for the token buffer condition variable to signal
            pthread_cond_wait(&condTokenBuffer, &lockTokenBuffer);
        }
        
        r = q1.front();
        // remove request from the Q1 buffer
        q1.pop();
        r->setQ1ExitTime();
        int i = 1;
        while(i++ <= inputData.tokenReq){
            if(inputData.logLevel == 2){
                int tokId = buffer.front();
                cout << "token number " << tokId << " being consumed by request r" << r->getRequestId() << endl;
            }
            // remove the tokens consumed by the request from the token buffer
            buffer.pop();
        }
        cout << "r" << r->getRequestId() << " leaves Q1, time  in Q1 = " << r->getTimeInQ1() << "ms, remaining token = " << buffer.size() << endl;
        
        // release the mutex lock for the token buffer
        pthread_mutex_unlock(&lockTokenBuffer);
        // release the mutex lock for the Q1 buffer
        pthread_mutex_unlock(&lockQ1);

        // acquire the mutex lock for Q2 buffer
        pthread_mutex_lock(&lockQ2);
        // add the request to the Q2 buffer
        q2.push(r);
        r->setQ2EntryTime();
        cout << "r" << r->getRequestId() << " enters Q2" << endl;
        
        // signal the server thread
        pthread_cond_signal(&condQ2);
        // release the mutex lock for the Q2 buffer
        pthread_mutex_unlock(&lockQ2);
    }

    return 0;
}