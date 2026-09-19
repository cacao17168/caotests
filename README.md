# caotests
This is simple testing framework.
# How to use
## 1. Clone repo
```sh
git clone https://github.com/cacao17168/caotests.git
cd caotests
```
## 2. Install dependencies
```sh
sudo apt install/pacman -S build-essential/base-devel
```
## 3. Execution
Add ```src/test_lib.h``` to your source file:
```c
#include "src/test_lib.h"
```
Register your test function:
```c
CT_TEST(your_function_name) {
    CT_ASSERT_EQ(expected_value, actual_value);
    CT_ASSERT_TRUE(cond_which_must_be_true);
    CT_ASSERT_FALSE(cond_which_must_be_false);
    return 0;
}
```
You need to change ```your_function_name```, ```expected_value```, ```actual_value```, ```cond_which_must_be_true```, ```cond_which_must_be_false``` to your conditions and data.
You don't have to use all asserts in 1 function. Use them as you need.
If you don't remove ```tests``` dir, ```test_framework``` will execute meta tests.

You can also use assertion functions as you want in your project.

Hope this will help u.
