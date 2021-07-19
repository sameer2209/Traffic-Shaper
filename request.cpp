#include "request.h"
#include "main.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <queue>

using namespace std;

std::queue<Request> q1;
pthread_mutex_t lockQ1;
pthread_cond_t condQ1;

int Request::requestCount = 0;

Request::Request(){
    requestId = requestCount + 1;
    q1EntryTime = 0;
    q1ExitTime = 0;
    q2EntryTime = 0;
    q2ExitTime = 0;
    serverExitTime = 0;
    requestCount++;
}

void Request::setQ1EntryTime(){
    time(&q1EntryTime);
}

void Request::setQ1ExitTime(){
    time(&q1ExitTime);
}

void Request::setQ2EntryTime(){
    time(&q2EntryTime);
}

void Request::setQ2ExitTime(){
    time(&q2ExitTime);
}

void Request::setServerExitTime(){
    time(&serverExitTime);
}

double Request::getTimeInQ1(){
    return difftime(q1ExitTime, q1EntryTime);
}

double Request::getTimeInQ2(){
    return difftime(q2ExitTime, q2EntryTime);
}

double Request::getServiceTime(){
    return difftime(serverExitTime, q2ExitTime);
}

double Request::getTimeInSystem(){
    return difftime(serverExitTime, q1EntryTime);
}

int Request::getRequestId(){
    return requestId;
}

void* startRequestThread(void* inputData){
    int requestRate = ((InputData*) inputData)->requestRate;
    int tokensReq = ((InputData*) inputData)->tokenReq;
    pthread_mutex_init(&lockQ1, 0);
    pthread_cond_init(&condQ1, 0);
    while(true){
        sleep(requestRate);
        Request req;
        pthread_mutex_lock(&lockQ1);
        cout << "r" << req.getRequestId() << " arrives, need " << tokensReq << " tokens" << endl;
        q1.push(req);
        req.setQ1EntryTime();
        cout << "r" << req.getRequestId() << " enters Q1" << endl;
        pthread_cond_signal(&condQ1);
        pthread_mutex_unlock(&lockQ1);
    }
}