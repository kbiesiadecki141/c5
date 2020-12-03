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
    float bump_r;
    float bump_cr;
    float bump_cl;
    float bump_l;

} C5Sensors_t;
#endif
