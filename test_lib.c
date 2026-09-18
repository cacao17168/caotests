#include "test_lib.h"
#include <stdio.h>

static int pass_ctr = 0;
static int fail_ctr = 0;
static int all_ctr = 0;

int ct_run_test(ct_test_func_t test, const char* name) {
    all_ctr++;
    printf(" %-45s ", name);
    fflush(stdout);
    int rc = test();
    if(rc == 0) {
        printf(GREEN "[PASS]\n" RESET);
        pass_ctr++;
        return 0;
    }
    fail_ctr++;
    return 1;
}

void ct_tests_report(void) {
    printf("Tests ended. In total: %d; Passed: %d; Failed: %d\n", all_ctr, pass_ctr, fail_ctr);
    return;
}

int main(int argc, char *argv[]) {

}
