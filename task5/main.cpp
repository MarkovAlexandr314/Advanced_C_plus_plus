#include <iostream>
#include <unistd.h>
#include "log.h"

using namespace std;

int main()
{
    Log *log = Log::Instance();
    log->message(LOG_NORMAL, "program loaded");
    usleep(500000);
    for (int i = 0; i < 10; i++)
    {
        log->message(LOG_NORMAL, "Messange number: " + to_string(i));
        usleep(500000);
    }

    log->message(LOG_WARNING, "WARNING!");
    log->message(LOG_ERROR, "error happens! help me !");
    log->print();

    return 0;
}