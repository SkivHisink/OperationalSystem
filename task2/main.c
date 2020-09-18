#include <sys/types.h>
#include <time.h>
#include <stdio.h>

int main()
{
    putenv("TZ=America/Los_Angeles");
    time_t la_time;
    time (&la_time);
    printf("%s", ctime(&la_time));
    return 0;
}
