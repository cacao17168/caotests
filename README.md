# caotests

A minimal, zero-dependency unit testing framework for C. Tests are registered
automatically via the linker, each test runs in its own forked process, and the
framework reports timing, memory usage, and crash signals — all in a single
small source file.

## Features

- **Automatic test registration** — declare a test with one macro, no manual
  lists or `main()` bookkeeping.
- **Process isolation** — every test runs in a `fork()`ed child process, so a
  segfault in one test never aborts the rest.
- **Crash reporting** — crashed tests are reported as `[CRASH]` with the signal
  number and name (e.g. `signal 11 (Segmentation fault)`).
- **Built-in assertions** — `CT_ASSERT_EQ`, `CT_ASSERT_TRUE`, `CT_ASSERT_FALSE`
  with file and line info on failure.
- **Leak & timing report** — in verbose mode each test reports its runtime and
  a memory diff (based on `/proc/self/statm`); a positive diff is flagged as a
  leak.
- **Output modes** — normal, `--verbose` (file, time, memory) and `--quiet`
  (silence the test output, only the summary).
- **Failure messages passed over a pipe** — messages produced in the child
  process survive its exit and are printed by the parent.

## Quick start

### Writing a test

```c
#include "test_lib.h"
#include "my_lib.h"

CT_TEST(test_addition) {
    CT_ASSERT_EQ(4, add(2, 2));
    return 0;
}

CT_TEST(test_null_check) {
    int *p = NULL;
    CT_ASSERT_TRUE(p == NULL);
    CT_ASSERT_FALSE(p != NULL);
    return 0;
}
```

The `CT_TEST(name)` macro:

1. declares the function `int name(void);`,
2. registers it in a special linker section `ct_tests`,
3. opens the function definition — just write the test body inside.

### Running the tests

The framework's `main()` walks the `ct_tests` linker section automatically, so
no registration code is needed. Build and run with:

```bash
make          # builds test_framework and fixture_tests
make test     # builds and runs both binaries
./test_framework --verbose
./test_framework --quiet
```

### Sample output

```
[RUN ] test_addition                              [1] [PASS]
[RUN ] test_null_check                            [2] [FAIL]
[RUN ] test_divide_by_zero                        [3] [CRASH] signal 8 (Floating point exception)
Tests ended. In total: 3; Passed: 1; Failed: 2
```

Verbose mode additionally prints the source file, per-test time, and a memory
diff (flagged red as `LEAK: +N byte` when positive).

## Available assertions

| Macro                          | Checks                                   |
|---|---|
| `CT_ASSERT_EQ(expected, actual)` | equality of two integer expressions      |
| `CT_ASSERT_TRUE(cond)`         | condition must be true                   |
| `CT_ASSERT_FALSE(cond)`        | condition must be false                  |

Failed assertions record a message (file, line, values) into `ct_fail_buf` and
make the test return `1`. To report a custom failure message from a test:

```c
ct_fail_record("unexpected state: %d\n", value);
return 1;
```

## Build system

The framework is expected to live in `third-party/caotests` of your project.
The makefile assembles:

- `test_framework` — the framework (`src/test_lib.c`) + meta tests
  (`tests/meta_tests.c`, `tests/flags_meta_tests.c`) + user tests from
  `../../user_tests/*.c`;
- `fixture_tests` — a small fixture binary (`tests/fixture.c`) with one passing
  and one failing test, used by the meta tests;
- header search paths for both the framework and your project's sources under
  `../../src`.

```bash
make              # build everything
make test         # build and run the suite
make clean        # remove binaries and objects
```

## Project layout

```
caotests/
├── makefile                  # build rules
├── src/
│   ├── test_lib.c            # framework implementation (runner, reporter)
│   └── test_lib.h            # public API: macros + ct_test_case_t
└── tests/
    ├── meta_tests.c          # framework's own test suite
    ├── flags_meta_tests.c    # tests for --verbose / --quiet modes
    └── fixture.c             # fixture binary for the flag tests
```

## How it works

- **Registration.** `CT_TEST` places a `ct_test_case_t` record into the custom
  linker section `ct_tests`. The runner iterates between `__start_ct_tests`
  and `__stop_ct_tests`, which the linker generates automatically.
- **Isolation.** `ct_run_test()` creates a pipe, forks, and runs the test in
  the child. After the test finishes, the child sends back the return code,
  elapsed nanoseconds, memory diff and accumulated failure text, then exits
  with `_exit()`. The parent reads the pipe, so results and failure messages
  are reported even if the child is killed.
- **Memory tracking.** Memory is sampled from `/proc/self/statm` before and
  after each test; a positive difference is reported as a leak in verbose mode.
- **Quiet mode.** With `--quiet`, each test still runs forked but stdout/stderr
  are redirected to `/dev/null` and only the final `Passed: N, Failed: M`
  summary is printed.
