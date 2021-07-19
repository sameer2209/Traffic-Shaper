#ifndef REQUEST_H
#define REQUEST_H

#include <ctime>

class Request {

    int requestId;
    time_t q1EntryTime;
    time_t q1ExitTime;
    time_t q2EntryTime;
    time_t q2ExitTime;
    time_t serverExitTime;

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