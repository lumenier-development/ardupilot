#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_MSP/msp.h>
#include <RC_Channel/RC_Channel.h>

#ifndef AP_CYCLOPS_ENABLED
#define AP_CYCLOPS_ENABLED 1
#endif

#ifndef AP_CYCLOPS_BACKEND_DEFAULT_ENABLED
#define AP_CYCLOPS_BACKEND_DEFAULT_ENABLED  AP_CYCLOPS_ENABLED
#endif

#ifndef HAL_MSP_CYCLOPS_ENABLED
#define HAL_MSP_CYCLOPS_ENABLED AP_CYCLOPS_BACKEND_DEFAULT_ENABLED && HAL_MSP_ENABLED
#endif

#define CYCLOPS_ANGLE_MAX               30
#define CYCLOPS_ANGLE_MIN               -30
#define CYCLOPS_ANGLE_RANGE             (CYCLOPS_ANGLE_MAX - CYCLOPS_ANGLE_MIN)

#define CYCLOPS_GRID_LENGTH             18
#define CYCLOPS_GRID_MID                9
#define CYCLOPS_GRID_START              0

#define CYCLOPS_MAX_TARGETS             3

typedef enum {
    CYCLOPS_SENSOR_AUTO = 0,
    CYCLOPS_SHORT_RANGE = 1,
    CYCLOPS_LONG_RANGE  = 2
} CYCLOPS_SENSOR_LIST;

typedef enum {
    CYCLOPS_NORMAL = 0x0,
    CYCLOPS_REBOOT = 0x1
} CYCLOPS_CMD_LIST;

typedef enum {
    ZERO_TARGETS_DETECTED = 0,
    ONE_TARGET_DETECTED = 6,
    TWO_TARGETS_DETECTED = 12, 
    THREE_TARGETS_DETECTED = 18
} CYCLOPS_NUM_TARGETS_DETECTED;

class AP_Cyclops
{
public:

    struct __attribute__((packed)) CyclopsRadarData {
        uint8_t target_number;
        int16_t distance_to_target;
        int16_t target_angle;
        uint8_t sensor_ID;
    };

    struct __attribute__((packed)) CyclopsCommand {
        uint16_t max_range;
        uint16_t min_range;
        uint8_t sensor_select;
        uint8_t mode;
    };

    AP_Cyclops();  // constructor

    void handle_msp(const MSP::msp_CyclopsRadarData_t *pkt, uint16_t packet_size);
    void rc_control(const RC_Channel::AuxSwitchPos ch_flag);

    void cyclops_debug();
    const CyclopsRadarData* get_data() const { return _data; }
    uint8_t get_payload_size(void);
    bool has_pending_command() const { return _command_pending; }
    CyclopsCommand consume_pending_command() { _command_pending = false; return _command; }
    const CyclopsCommand* get_command() const { return &_command; }

    static AP_Cyclops *get_singleton() { return _singleton; }

    // parameter var info table
    static const AP_Param::GroupInfo var_info[];

private:
    static AP_Cyclops *_singleton;

    // example param (optional but typical)
    AP_Int8 _enable;

    CyclopsRadarData    _data[3]{0, 1, 0, 0, 
                                 0, 0, 0, 0,
                                 0, 0, 0, 0};

    CyclopsCommand      _command{0, 0, 1, 0};
    bool                _command_pending = false;

    uint8_t _payload_size = 0;
};

namespace AP {
    AP_Cyclops *cyclops();
}