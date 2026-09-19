CC = gcc
FLAGS = -g -O0 -Wall -Wextra
INCLUDES = -Isrc/

all: meta_tests fixture_tests

# framework + meta tests
meta_tests: src/test_lib.c tests/meta_tests.c tests/flags_meta_tests.c src/test_lib.h
	$(CC) $(FLAGS) $(INCLUDES) src/test_lib.c tests/meta_tests.c tests/flags_meta_tests.c -o meta_tests

# fixture for flags tests(1 complete and 1 fail)
fixture_tests: src/test_lib.c tests/fixture.c src/test_lib.h
	$(CC) $(FLAGS) $(INCLUDES) src/test_lib.c tests/fixture.c -o fixture_tests

# make all and launch meta tests
test: all
	./meta_tests

clean:
	rm -f meta_tests fixture_tests

.PHONY: all test clean
