#include "test_lib.h"
#include <stdio.h>
#include <string.h>

static int pass_ctr = 0;
static int fail_ctr = 0;
static int all_ctr = 0;

int ct_run_test(ct_test_func_t test, const char* name, int verbose) {
    struct timespec start, end;

    all_ctr++;
    printf("[RUN ] %-45s [%d]", name, all_ctr);
    fflush(stdout);

    clock_gettime(CLOCK_REALTIME, &start); //function start time
    int rc = test();
    clock_gettime(CLOCK_REALTIME, &end); //function end time
    double elapsed_ns = (double)(end.tv_sec - start.tv_sec) * 1e9 + (double)(end.tv_nsec - start.tv_nsec); //calculate function duration

    if(rc == 0) {
        printf(GREEN "[PASS]\n" RESET);
        if(!verbose) printf("%-45s, took %f sec\n", name, elapsed_ns); //verbose output
        pass_ctr++;
        return 0;
    }

    if(!verbose) printf("%-45s, took %f sec\n", name, elapsed_ns); //verbose output
    fail_ctr++;
    return 1;
}

void ct_tests_report(struct timespec start, struct timespec end, int verbose) {
    printf("Tests ended. In total: %d; Passed: %d; Failed: %d\n", all_ctr, pass_ctr, fail_ctr);

    double elapsed_ns = (double)(end.tv_sec - start.tv_sec) * 1e9 + (double)(end.tv_nsec - start.tv_nsec); //calculate tests duration
    if(!verbose) printf("Time spend: %f\n", elapsed_ns); //verbose output

    return;
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int quiet = 0;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--verbose") == 0) {
            printf("Verbose mod enabled. Extending output\n");
            verbose = 1;
        }

        if(strcmp(argv[i], "--quiet") == 0) quiet = 1;

        if((verbose == 1) && (quiet == 1)) {
            printf(RED "[ERROR]" RESET "Can't use both args at the same time\n");
        }
    }

}
