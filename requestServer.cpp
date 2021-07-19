#include "requestServer.h"
#include "main.h"
#include "request.h"
#include <queue>
#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

queue<Request> q2;
pthread_mutex_t lockQ2;
pthread_cond_t condQ2;

void* startServerThread(void* inputData){
    int reqServiceTime = ((InputData*) inputData)->requestServiceTime;
    pthread_mutex_init(&lockQ2, 0);
    pthread_cond_init(&condQ2, 0);

    while(true){
        pthread_mutex_lock(&lockQ2);
        while(q2.empty())
            pthread_cond_wait(&condQ2, &lockQ2);
        Request r = q2.front();
        q2.pop();
        r.setQ2ExitTime();
        cout << "r" << r.getRequestId() << " leaves Q2, time in Q2 = " << r.getTimeInQ2() << endl;
        cout << "r" << r.getRequestId() << " begins service at S, requesting " << reqServiceTime << " of service" << endl;
        sleep(reqServiceTime);
        r.setServerExitTime();
        cout << "r" << r.getRequestId() << " departs from S, service time = " << r.getServiceTime() << " time in system = " << r.getTimeInSystem() << endl;
        pthread_mutex_unlock(&lockQ2);
    }

    pthread_mutex_destroy(&lockQ2);
    pthread_cond_destroy(&condQ2);
}