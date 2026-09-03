#ifndef HASHRATE_CHART_SCALE_H
#define HASHRATE_CHART_SCALE_H

#include <stdint.h>
#include <stdbool.h>

#define HASHRATE_CHART_MAX_VALUE 30000.0f

typedef struct
{
    int32_t minimum;
    int32_t maximum;
    int32_t step;
} hashrate_chart_scale_t;

void hashrate_chart_scale_calculate(float minimum_value,
                                    float maximum_value,
                                    hashrate_chart_scale_t *scale);

bool hashrate_chart_value_parse(const char *text, float *value);

#endif // HASHRATE_CHART_SCALE_H
