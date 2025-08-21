#include <stdio.h>

#define ABC_IMPLEMENTATION
#include "../abc.h"
#include "tests.h"

int main(void)
{
    test_fibonacci();
    test_pie();
    abc_clock_central_free();
    return 0;
}

