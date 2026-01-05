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

void AP_Cyclops::handle_msp(const MSP::msp_CyclopsRecvData_t &pkt)
{
    static_assert(sizeof(CyclopsRecvData) == 5, "CyclopsRecvData must be packed");
    // pkt is a reference, not pointer!
    // C++ is confusing, why don't we just use C! :(
    _data.num_targets_detected = pkt.num_targets_detected;
    _data.target_angle         = pkt.target_angle;
    _data.distance_to_target   = pkt.distance_to_target;
}

void AP_Cyclops::cyclops_debug()
{
    _data.num_targets_detected++;
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
