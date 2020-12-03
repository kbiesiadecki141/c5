#ifndef _C5SENSORTYPES_H
#define _C5SENSORTYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{

    // e-stop switch
    bool button;

    // Raw value of ultrasonic sensors
    float us_f;
    float us_rf;
    float us_lf;
    float us_rb;
    float us_lb;

    // Raw value of bump sensors
    bool bump_r;
    bool bump_cr;
    bool bump_cl;
    bool bump_l;

} C5Sensors_t;
#endif
