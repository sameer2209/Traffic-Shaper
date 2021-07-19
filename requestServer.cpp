#include "requestServer.h"
#include "main.h"
#include "request.h"
#include <queue>
#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

queue<Request*> q2;
pthread_mutex_t lockQ2;
pthread_cond_t condQ2;

void* startServerThread(void* inputData){
    int reqServiceTime = ((InputData*) inputData)->requestServiceTime;
    pthread_mutex_init(&lockQ2, 0);
    pthread_cond_init(&condQ2, 0);
    Request* req;
    while(true){
        pthread_mutex_lock(&lockQ2);
        while(q2.empty())
            pthread_cond_wait(&condQ2, &lockQ2);
        req = q2.front();
        q2.pop();
        req->setQ2ExitTime();
        cout << "r" << req->getRequestId() << " leaves Q2, time in Q2 = " << req->getTimeInQ2() << endl;
        cout << "r" << req->getRequestId() << " begins service at S, requesting " << reqServiceTime << " of service" << endl;
        sleep(reqServiceTime);
        req->setServerExitTime();
        cout << "r" << req->getRequestId() << " departs from S, service time = " << req->getServiceTime() << " time in system = " << req->getTimeInSystem() << endl;
        pthread_mutex_unlock(&lockQ2);
        free(req);
    }

    pthread_mutex_destroy(&lockQ2);
    pthread_cond_destroy(&condQ2);
}