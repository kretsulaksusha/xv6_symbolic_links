#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

int
main(void)
{
    struct rtcdate r;
    date(&r);
    // A file descriptor is a small integer representing a kernel-managed object that
    // a process may read from or write to.
    // By convention, a process reads from file descriptor 0 (standard input), writes
    // output to file descriptor 1 (standard output), and writes error messages to
    // file descriptor 2 (standard error).
    // So, fd = 1 is synonymous with 'stdout'.
    printf(1, "%d:%d:%d %d.%d.%d\n", r.hour, r.minute, r.second, r.day, r.month, r.year);
    exit();
}
