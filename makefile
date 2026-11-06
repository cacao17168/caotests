CC = gcc
CFLAGS = -g -O0 -Wall -Wextra

INCLUDES = -Isrc/ $(USER_INCLUDES)

SRC_DIR = src
TESTS_DIR = tests

SRC = $(SRC_DIR)/test_lib.c $(TESTS_DIR)/meta_tests.c $(TESTS_DIR)/flags_meta_tests.c
SRC_FIXT = $(SRC_DIR)/test_lib.c $(TESTS_DIR)/fixture.c

all: test_framework fixture_tests

# framework + meta tests
test_framework: $(SRC)
	$(CC) $^ $(INCLUDES) -o $@

# fixture for flags tests(1 complete and 1 fail)
fixture_tests: $(SRC_FIXT)
	$(CC) $^ $(INCLUDES) -o $@

# make all and launch meta tests
test: all
	./test_framework && ./fixture_tests

clean:
	rm -f test_framework fixture_tests

.PHONY: all test clean
