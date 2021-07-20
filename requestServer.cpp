#include "requestServer.h"
#include "trafficShaper.h"
#include "request.h"
#include <queue>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <iomanip>

using namespace std;

queue<Request*> q2;
// mutex lock for the Q2 buffer
pthread_mutex_t lockQ2;
// condition variable for the Q2 buffer
pthread_cond_t condQ2;

void* startServerThread(void* inputData){
    int reqServiceTime = ((InputData*) inputData)->requestServiceTime;
    pthread_mutex_init(&lockQ2, 0);
    pthread_cond_init(&condQ2, 0);
    Request* req;
    while(true){
        // acquire the mutex lock for the Q2 buffer
        pthread_mutex_lock(&lockQ2);
        while(q2.empty()){
            // server thread waiting for the Q2 condition variable
            pthread_cond_wait(&condQ2, &lockQ2);
        }
        req = q2.front();
        // remove the request from the Q2 buffer
        q2.pop();
        req->setQ2ExitTime();
        cout << fixed << setprecision(3) << "r" << req->getRequestId() << " leaves Q2, time in Q2 = " << req->getTimeInQ2() << "ms" << endl;
        cout << fixed << setprecision(3) << "r" << req->getRequestId() << " begins service at S, requesting " << reqServiceTime * 1000.0 << "ms of service" << endl;
        
        // service the request by the server thread
        sleep(reqServiceTime);
        req->setServerExitTime();
        cout << fixed << setprecision(3) << "r" << req->getRequestId() << " departs from S, service time = " << req->getServiceTime() << "ms, time in system = " << req->getTimeInSystem() << "ms" << endl;
        // release the mutex lock for the Q2 buffer
        pthread_mutex_unlock(&lockQ2);

        // delete the request object once it is serviced to avoid memory leaks
        free(req);
    }
}