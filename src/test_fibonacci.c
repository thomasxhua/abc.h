#include "tests.h"

#include <stdint.h>

uint64_t fibonacci(uint64_t n)
{
    return (n <= 1) ? 1 : fibonacci(n-1) + fibonacci(n-2);
}

void test_fibonacci(void)
{
    const abc_clock_t clock = abc_clock_register("fibonacci");
    abc_clock_start(clock);
    ABC_LOGVAR("%llu", fibonacci(40));
    abc_clock_log_round(clock);
    abc_clock_stop(clock);
}

