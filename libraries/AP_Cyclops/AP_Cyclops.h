#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_MSP/msp.h>

#ifndef AP_CYCLOPS_ENABLED
#define AP_CYCLOPS_ENABLED 1
#endif

#ifndef AP_CYCLOPS_BACKEND_DEFAULT_ENABLED
#define AP_CYCLOPS_BACKEND_DEFAULT_ENABLED  AP_CYCLOPS_ENABLED
#endif

#ifndef HAL_MSP_CYCLOPS_ENABLED
#define HAL_MSP_CYCLOPS_ENABLED AP_CYCLOPS_BACKEND_DEFAULT_ENABLED && HAL_MSP_ENABLED
#endif

#define CYCLOPS_ANGLE_MAX               90
#define CYCLOPS_ANGLE_MIN               -90

#define CYCLOPS_GRID_LENGTH             18
#define CYCLOPS_GRID_MID                9
#define CYCLOPS_GRID_START              0

typedef enum {
    NUM_TARGETS_DETECTED = 0,
    TARGET_ANGLE = 1,
    DISTANCE_TO_TARGET = 2,
    LIST_MAX = 3
} CYCLOPS_DATA_LIST;

class AP_Cyclops
{
public:

    struct __attribute__((packed)) CyclopsRecvData {
        int16_t target_angle;
        int16_t distance_to_target;
        uint8_t num_targets_detected;
    };

    AP_Cyclops();  // constructor

    void handle_msp(const MSP::msp_CyclopsRecvData_t &pkt);

    void cyclops_debug();
    CyclopsRecvData get_data() const { return _data; }

    static AP_Cyclops *get_singleton() { return _singleton; }

    // parameter var info table
    static const AP_Param::GroupInfo var_info[];

private:
    static AP_Cyclops *_singleton;

    // example param (optional but typical)
    AP_Int8 _enable;

    CyclopsRecvData _data{0, 0, 0};
};

namespace AP {
    AP_Cyclops *cyclops();
}