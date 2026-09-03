#include "hashrate_chart_scale.h"

#include <assert.h>
#include <stdio.h>

static void assert_scale(float data_minimum,
                         float data_maximum,
                         int32_t expected_minimum,
                         int32_t expected_maximum,
                         int32_t expected_step)
{
    hashrate_chart_scale_t scale;
    hashrate_chart_scale_calculate(data_minimum, data_maximum, &scale);

    assert(scale.minimum == expected_minimum);
    assert(scale.maximum == expected_maximum);
    assert(scale.step == expected_step);
    assert(scale.minimum <= data_minimum);
    assert(scale.maximum >= data_maximum);
    assert(scale.maximum - scale.minimum == scale.step * 5);
}

int main(void)
{
    assert_scale(1986.0f, 2584.0f, 1950, 2700, 150);
    assert_scale(2105.0f, 2464.0f, 2000, 2500, 100);
    assert_scale(2053.0f, 2053.0f, 2030, 2080, 10);
    assert_scale(0.0f, 100.0f, 0, 125, 25);

    puts("hashrate chart scale tests passed");
    return 0;
}
