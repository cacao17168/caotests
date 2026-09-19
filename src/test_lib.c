#include "test_lib.h"
#include <bits/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static int pass_ctr = 0;
static int fail_ctr = 0;
static int all_ctr = 0;

long long get_current_memory_bytes(void) {
    FILE* file = fopen("/proc/self/statm", "r");
    if(!file) return 0;

    long pages = 0;
    if(fscanf(file, "%ld", &pages) != 1) {
        fclose(file);
        return 0;
    }
    fclose(file);

    long page_size = sysconf(_SC_PAGESIZE);
    return (long long)pages * page_size;
}

int ct_run_test(ct_test_func_t test, const char* name, const char* file, int verbose) {
    struct timespec start, end;

    all_ctr++;
    printf("[RUN ] %-45s [%d]", name, all_ctr);
    fflush(stdout);

    long long mem_bef = get_current_memory_bytes();
    clock_gettime(CLOCK_REALTIME, &start); //function start time

    int rc = test();

    clock_gettime(CLOCK_REALTIME, &end); //function end time
    long long mem_aft = get_current_memory_bytes();
    long long mem_diff = mem_aft - mem_bef;

    double elapsed_ns = (double)(end.tv_sec - start.tv_sec) * 1e9 + (double)(end.tv_nsec - start.tv_nsec); //calculate function duration

    if(verbose) printf(" %s ", file);

    if(rc == 0) {
        if(!verbose) {
            printf(GREEN "[PASS]\n" RESET);
        } else {
            printf(GREEN "[PASS]" RESET " %-38s | Time: %.6f сек", name, elapsed_ns / 1e9); //verbose output
            if(mem_diff > 0) {
                printf(RED " | LEAK: +%lld byte" RESET "\n", mem_diff);
            } else {
                printf(" | Mem: ok\n");
            }
        }
        pass_ctr++;
        return 0;
    }

    if(verbose) printf("(%-45s, took %f sec)\n", name, elapsed_ns); //verbose output
    fail_ctr++;
    return 1;
}

void ct_tests_report(struct timespec start, struct timespec end, int verbose) {
    printf("Tests ended. In total: %d; Passed: %d; Failed: %d\n", all_ctr, pass_ctr, fail_ctr);

    double elapsed_ns = (double)(end.tv_sec - start.tv_sec) * 1e9 + (double)(end.tv_nsec - start.tv_nsec); //calculate tests duration
    if(verbose) printf("Time spend: %f\n", elapsed_ns); //verbose output

    return;
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int quiet = 0;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--verbose") == 0) verbose = 1;
        if(strcmp(argv[i], "--quiet") == 0) quiet = 1;

        if((verbose) && (quiet))
            printf(RED "[ERROR]" RESET "Can't use both args at the same time\n");

        if(verbose)
            printf("Verbose mode enabled. Extending output.\n");
    }

    ct_test_case_t *start = &__start_ct_tests;
    ct_test_case_t *stop = &__stop_ct_tests;

    struct timespec total_start, total_end;
    clock_gettime(CLOCK_REALTIME, &total_start);

    for(ct_test_case_t *t = start; t < stop; t++) {
        if(!quiet) {
            ct_run_test(t->func, t->name, t->file, verbose);
        } else {
            int rc = t->func();
            all_ctr++;
            if(rc == 0) pass_ctr++; else fail_ctr++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &total_end);

    if(!quiet) {
        ct_tests_report(total_start, total_end, verbose);
    } else {
        printf("Passed: %d, Failed%d\n", pass_ctr, fail_ctr);
    }

    return (fail_ctr > 0) ? 1 : 0;
}
