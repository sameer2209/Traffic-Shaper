#include "request.h"
#include "trafficShaper.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <queue>

using namespace std;

std::queue<Request*> q1;
pthread_mutex_t lockQ1;
pthread_cond_t condQ1;

int Request::requestCount = 0;

Request::Request(){
    requestId = requestCount + 1;
    requestCount++;
}

void Request::setQ1EntryTime(){
    q1EntryTime = chrono::high_resolution_clock::now();
}

void Request::setQ1ExitTime(){
    q1ExitTime = chrono::high_resolution_clock::now();
}

void Request::setQ2EntryTime(){
    q2EntryTime = chrono::high_resolution_clock::now();
}

void Request::setQ2ExitTime(){
    q2ExitTime = chrono::high_resolution_clock::now();
}

void Request::setServerExitTime(){
    serverExitTime = chrono::high_resolution_clock::now();
}

double Request::getTimeInQ1(){
    chrono::duration<double, milli> tm = q1ExitTime - q1EntryTime;
    return tm.count();
}

double Request::getTimeInQ2(){
    chrono::duration<double, milli> tm = q2ExitTime - q2EntryTime;
    return tm.count();
}

double Request::getServiceTime(){
    chrono::duration<double, milli> tm = serverExitTime - q2ExitTime;
    return tm.count();
}

double Request::getTimeInSystem(){
    chrono::duration<double, milli> tm = serverExitTime - q1EntryTime;
    return tm.count();
}

int Request::getRequestId(){
    return requestId;
}

void* startRequestThread(void* inputData){
    int requestRate = ((InputData*) inputData)->requestRate;
    int tokensReq = ((InputData*) inputData)->tokenReq;
    int bufferLen = ((InputData*) inputData)->tokenBufferSize;
    pthread_mutex_init(&lockQ1, 0);
    pthread_cond_init(&condQ1, 0);
    Request* req;
    while(true){
        sleep(requestRate);
        req = new Request();
        pthread_mutex_lock(&lockQ1);
        cout << "r" << req->getRequestId() << " arrives, need " << tokensReq << " tokens" << endl;
        if(tokensReq > bufferLen){
            cout << "r" << req->getRequestId() << " resource requirement is more than token buffer length, dropping request r" << req->getRequestId() << endl;
            cout << "requests can not be served, terminating program..." << endl;
            cout << "emulation ends" << endl;
            exit(0);
        }
        q1.push(req);
        req->setQ1EntryTime();
        cout << "r" << req->getRequestId() << " enters Q1" << endl;
        pthread_cond_signal(&condQ1);
        pthread_mutex_unlock(&lockQ1);
    }
}