#include "main.h"

#ifdef DEBUG
#include <mcheck.h>

#include <execinfo.h>
#define MAX_STACK_DEPTH (20)
static void handle_sig(int sig)
{
    printf("sig recv:%d\n", sig);
    if (sig == SIGSEGV || sig == SIGABRT)
    {
        void *buffer[MAX_STACK_DEPTH];
        int depth = backtrace(buffer, MAX_STACK_DEPTH);
        printf("Depth: %d\n", depth);
        char **func_names = backtrace_symbols(buffer, depth);
        for (int i = 0; i < depth; i++)
        {
            printf("Depth: %d, func name: %s\n", i, func_names[i]);
        }
        exit(-1);
    }
}
#endif
int main(int argc, char **argv)
{
#ifdef DEBUG
    printf("debug app main start");
    setenv("MALLOC_TRACE", "./memleak.log", 1);
    mtrace();

    struct sigaction act;
    act.sa_flags = SA_RESTART;
    act.sa_handler = handle_sig;
    sigemptyset(&act.sa_mask);
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGABRT, &act, NULL);
#endif
    mlog_init();

    lvgl_roki_main();
    return 0;
}
