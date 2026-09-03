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

static int32_t map_value_to_y(int32_t value,
                              const hashrate_chart_scale_t *scale,
                              int32_t chart_height)
{
    int32_t offset = (value - scale->minimum) * chart_height;
    return chart_height - (offset / (scale->maximum - scale->minimum));
}

static void assert_rounded_range_controls_positions(void)
{
    hashrate_chart_scale_t scale;
    hashrate_chart_scale_calculate(1986.0f, 2584.0f, &scale);

    /* LVGL maps both tick labels and series points through the configured
     * minimum/maximum. Verify the asymmetric rounded range drives their
     * positions instead of only changing the displayed label text. */
    const int32_t chart_height = 250;
    for (int32_t tick = 0; tick < 6; tick++)
    {
        int32_t tick_value = scale.maximum - (tick * scale.step);
        assert(map_value_to_y(tick_value, &scale, chart_height) == tick * 50);
    }

    assert(map_value_to_y(1986, &scale, chart_height) == 238);
    assert(map_value_to_y(2250, &scale, chart_height) == 150);
    assert(map_value_to_y(2584, &scale, chart_height) == 39);
}

int main(void)
{
    assert_scale(1986.0f, 2584.0f, 1950, 2700, 150);
    assert_scale(2105.0f, 2464.0f, 2000, 2500, 100);
    assert_scale(2053.0f, 2053.0f, 2030, 2080, 10);
    assert_scale(0.0f, 100.0f, 0, 125, 25);
    assert_rounded_range_controls_positions();

    puts("hashrate chart scale tests passed");
    return 0;
}
