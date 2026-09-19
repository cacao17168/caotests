#ifndef TEST_LIB_H_
#define TEST_LIB_H_
#include <time.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

#define CT_ASSERT_EQ(expected, actual) \
    do {\
        if((expected) != (actual)) {\
            printf(RED "[FAIL]" RESET "%s:%d: expected %d, actual %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual));\
            return 1;\
        }\
    } while (0)

#define CT_ASSERT_TRUE(cond) \
    do {\
        if(!(cond)) {\
            printf(RED "[FAIL]" RESET "%s: %d: asserting true condition failed %s\n", __FILE__, __LINE__, #cond);\
            return 1;\
        }\
    } while(0)

#define CT_ASSERT_FALSE(cond) \
    do {\
        if((cond)) {\
            printf(RED "[FAIL]" RESET "%s: %d: asserting false condition failed %s\n", __FILE__, __LINE__, #cond);\
            return 1;\
        }\
    } while(0)

typedef int (*ct_test_func_t)(void); //test function type

int ct_run_test(ct_test_func_t test, const char* name, int verbose); //execute test function
void ct_tests_report(struct timespec start, struct timespec end, int verbose); //display tests results

#endif
