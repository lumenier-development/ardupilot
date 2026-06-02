#pragma once

#include "PL_K_config.h"

#if AP_PL_K_ENABLED

#include <AP_HAL/AP_HAL.h>
#include <AP_SerialManager/AP_SerialManager.h>

class PL_K
{
public:
    PL_K();

    CLASS_NO_COPY(PL_K);

    static PL_K *get_singleton() { return _singleton; }

    void init();
    void update();

private:
    static PL_K *_singleton;

    AP_HAL::UARTDriver *_uart = nullptr;

    // line buffer for assembling incoming ASCII messages
    static const uint8_t LINE_BUF_SIZE = 128;
    char _line_buf[LINE_BUF_SIZE];
    uint8_t _line_len = 0;

    void parse_line(const char *line);

    // buffer holding the most recent 8-character payload, null-terminated
    static const uint8_t PAYLOAD_SIZE = 16;
    char _payload[PAYLOAD_SIZE + 1] = "NO PAYLOAD";

public:
    const char *get_payload() const { return _payload; }
};

#endif  // AP_PL_K_ENABLED
