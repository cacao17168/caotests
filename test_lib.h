#ifndef TEST_LIB_H_
#define TEST_LIB_H_

#define CT_ASSERT_EQ(expected, actual) \
    do {\
        if((expected) != (actual)) {\
            printf("[FAIL] %s:%d: expected %d, actual %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual));\
            return 1;\
        }\
    } while (0)

#define CT_ASSERT_COND(cond) \
    do {\
        if(!(cond)) {\
            printf("[FAIL] %s: %d: asserting failed %s\n", __FILE__, __LINE__, #cond);\
            return 1;\
        }\
    } while(0)

typedef int (*ct_test_func_t)(void); //test function type

int ct_run_test(ct_test_func_t test, const char* name); //execute test function
void ct_tests_report(void); //display tests results

#endif
