#include "AP_Cyclops.h"
#include <AP_HAL/AP_HAL.h>

AP_Cyclops *AP_Cyclops::_singleton = nullptr;

AP_Cyclops::AP_Cyclops()
{
    if (_singleton != nullptr) {
#if CONFIG_HAL_BOARD == HAL_BOARD_SITL
        AP_HAL::panic("AP_Cyclops must be singleton");
#endif
        return;
    }
    _singleton = this;
    AP_Param::setup_object_defaults(this, var_info);
}

void AP_Cyclops::handle_msp(const MSP::msp_CyclopsRadarData_t *pkt, uint16_t packet_size)
{
    static_assert(sizeof(CyclopsRadarData) == 6, "CyclopsRadarData must be packed");

    _payload_size = packet_size;

    switch (packet_size) {
        case THREE_TARGETS_DETECTED:
            _data[2].target_number         = pkt[2].target_number;
            _data[2].distance_to_target    = pkt[2].distance_to_target;
            _data[2].target_angle          = pkt[2].target_angle;
            _data[2].sensor_ID             = pkt[2].sensor_ID;
            [[fallthrough]];
        case TWO_TARGETS_DETECTED:
            _data[1].target_number         = pkt[1].target_number;
            _data[1].distance_to_target    = pkt[1].distance_to_target;
            _data[1].target_angle          = pkt[1].target_angle;
            _data[1].sensor_ID             = pkt[1].sensor_ID;
            [[fallthrough]];
        case ONE_TARGET_DETECTED:
            _data[0].target_number         = pkt[0].target_number;
            _data[0].distance_to_target    = pkt[0].distance_to_target;
            _data[0].target_angle          = pkt[0].target_angle;
            _data[0].sensor_ID             = pkt[0].sensor_ID;
            break;
        default:
            break;
    }

    /**
     * If a target field is not sent, 
     * zero out the field so we know the target is not detected.
     */
    switch (packet_size) {
        case ZERO_TARGETS_DETECTED:
            _data[0].target_number         = CYCLOPS_NORMAL;
            _data[0].distance_to_target    = CYCLOPS_NORMAL;
            _data[0].target_angle          = CYCLOPS_NORMAL;
            _data[0].sensor_ID             = CYCLOPS_SENSOR_AUTO;
            [[fallthrough]];
        case ONE_TARGET_DETECTED:
            _data[1].target_number         = CYCLOPS_NORMAL;
            _data[1].distance_to_target    = CYCLOPS_NORMAL;
            _data[1].target_angle          = CYCLOPS_NORMAL;
            _data[1].sensor_ID             = CYCLOPS_SENSOR_AUTO;
            [[fallthrough]];
        case TWO_TARGETS_DETECTED:
            _data[2].target_number         = CYCLOPS_NORMAL;
            _data[2].distance_to_target    = CYCLOPS_NORMAL;
            _data[2].target_angle          = CYCLOPS_NORMAL;
            _data[2].sensor_ID             = CYCLOPS_SENSOR_AUTO;
            break;
        default:
            break;
    }
}

void AP_Cyclops::rc_control(const RC_Channel::AuxSwitchPos ch_flag)
{
    static_assert(sizeof(CyclopsCommand) == 7, "CyclopsCommand must be packed");
    switch (ch_flag) {
        case RC_Channel::AuxSwitchPos::HIGH:
            _command.sensor_ID = CYCLOPS_LONG_RANGE;
            _command.sensor_select = CYCLOPS_LONG_RANGE;
            break;
        default:
            _command.sensor_ID = CYCLOPS_SHORT_RANGE;
            _command.sensor_select = CYCLOPS_SHORT_RANGE;
            break;
    }
    _command_pending = true;
}

uint8_t AP_Cyclops::get_payload_size(void) {
    return _payload_size;
}

void AP_Cyclops::cyclops_debug()
{
    _data[0].sensor_ID++;
}

const AP_Param::GroupInfo AP_Cyclops::var_info[] = {
    // Minimal valid table; add real params later if you want.
    // Example (uncomment if you want an ENABLE param):
    AP_GROUPINFO("ENABLE", 1, AP_Cyclops, _enable, 1),
    AP_GROUPEND
};

namespace AP {
    AP_Cyclops *cyclops()
    {
        return AP_Cyclops::get_singleton();
    }
}
