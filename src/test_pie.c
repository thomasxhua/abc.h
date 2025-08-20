#include "tests.h"

#include <stdint.h>

static abc_clock_t clock_sqrt;
static abc_clock_t clock_sqr;

uint64_t linear_sqrt(uint64_t n)
{
    abc_clock_start(clock_sqrt);
    uint64_t res = -1ULL;
    for (volatile uint64_t i=0; i<n; ++i)
    {
        if (i*i >= n)
        {
            res = i;
            break;
        }
    }
    abc_clock_stop(clock_sqrt);
    return res;
}

uint64_t linear_sqr(uint64_t n)
{
    abc_clock_start(clock_sqr);
    uint64_t sum = 0;
    for (volatile uint64_t i=0; i<n; ++i)
        sum += n;
    abc_clock_stop(clock_sqr);
    return sum;
}

uint64_t quadratic_sqr(uint64_t n)
{
    abc_clock_start(clock_sqr);
    uint64_t sum = 0;
    for (volatile uint64_t i=0; i<n; ++i)
        for (volatile uint64_t j=0; j<n; ++j)
            sum += 1;
    abc_clock_stop(clock_sqr);
    return sum;
}


void test_pie(void)
{
    // register static clocks
    clock_sqrt = abc_clock_register("clock_sqrt");
    clock_sqr  = abc_clock_register("clock_sqr");

    // prepare test
    const uint64_t m = 3000;
    uint64_t n;

    // test 1
    ABC_LOG("using linear_sqr");
    n = 2;
    for (uint64_t i=0; i<m; ++i)
    {
        n = (i%2 == 0)
            ? linear_sqr(n)
            : linear_sqrt(n);
        n += 1;
    }
    ABC_LOGVAR("%llu", n);
    abc_pie_chart(clock_sqrt, clock_sqr);

    // reset clocks
    abc_clock_reset(clock_sqrt);
    abc_clock_reset(clock_sqr);

    // test 2
    ABC_LOG("using quadratic_sqr");
    n = 2;
    for (uint64_t i=0; i<m; ++i)
    {
        n = (i%2 == 0)
            ? quadratic_sqr(n)
            : linear_sqrt(n);
        n += 1;
    }
    ABC_LOGVAR("%llu", n);
    abc_pie_chart(clock_sqrt, clock_sqr);
}

