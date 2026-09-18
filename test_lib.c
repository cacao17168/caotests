#include "test_lib.h"
#include <stdio.h>

int run_test(test_func_t test, const char* name) {
    printf(" %-45s ", name);
    fflush(stdout);
    int rc = test();
    if(rc == 0) {
        printf("[PASS]\n");
        return 0;
    }
    return 1;
}
