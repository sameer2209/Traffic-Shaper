#ifndef REQUEST_H
#define REQUEST_H

#include <chrono>

class Request {

    int requestId;
    std::chrono::high_resolution_clock::time_point q1EntryTime;
    std::chrono::high_resolution_clock::time_point q1ExitTime;
    std::chrono::high_resolution_clock::time_point q2EntryTime;
    std::chrono::high_resolution_clock::time_point q2ExitTime;
    std::chrono::high_resolution_clock::time_point serverExitTime;

    public:
        static int requestCount;
        Request();
        int getRequestId();
        void setQ1EntryTime();
        void setQ1ExitTime();
        void setQ2EntryTime();
        void setQ2ExitTime();
        void setServerExitTime();
        double getTimeInQ1();
        double getTimeInQ2();
        double getServiceTime();
        double getTimeInSystem();
};

void* startRequestThread(void* inputData);

#endif