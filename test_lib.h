#ifndef TEST_LIB_H_
#define TEST_LIB_H_

#define ASSERT_EQ(expected, actual) \
    do {\
        if((expected) != (actual)) {\
            printf("[FAIL] %s:%d: expected %d, actual %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual));\
            return 1;\
        }\
    } while (0)

#define ASSERT_COND(cond) \
    do {\
        if(!(cond)) {\
            printf("[FAIL] %s: %d: asserting failed %s\n", __FILE__, __LINE__, #cond);\
            return 1;\
        }\
    } while(0)

typedef int (*test_func_t)(void);

int run_test(test_func_t test, const char* name);

#endif
