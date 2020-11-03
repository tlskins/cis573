#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

void timer_handler (int signum)
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("%s",asctime(timeinfo));
}

int main ()
{
    struct sigaction sa;
    struct itimerval timer;

    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa, NULL);

    timer.it_value.tv_sec = 1;
    timer.it_interval.tv_sec = 1;
    setitimer (ITIMER_VIRTUAL, &timer, NULL);
    while (1);
}