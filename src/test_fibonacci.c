#include "tests.h"

#include <stdint.h>

#include "../abc.h"

#define TEST_BASIC_FIBONACCI

uint64_t fibonacci(uint64_t n)
{
    return (n <= 1) ? 1 : fibonacci(n-1) + fibonacci(n-2);
}

void test_fibonacci(void)
{
    const abc_clock_t id = abc_clock_register("fibonacci");
    abc_clock_start(id);
    ABC_LOGVAR("%llu", fibonacci(10));
    abc_clock_log(id);
    ABC_LOGVAR("%llu", fibonacci(20));
    abc_clock_log(id);
    ABC_LOGVAR("%llu", fibonacci(30));
    abc_clock_log(id);
    ABC_LOGVAR("%llu", fibonacci(40));
    abc_clock_log(id);
    abc_clock_stop(id);
}

