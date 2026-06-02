#include "PL_K.h"

#if AP_PL_K_ENABLED

PL_K *PL_K::_singleton;

PL_K::PL_K()
{
    _singleton = this;
}

void PL_K::init()
{
    const AP_SerialManager &serial_manager = AP::serialmanager();
    _uart = serial_manager.find_serial(AP_SerialManager::SerialProtocol_PL_K, 0);
    if (_uart == nullptr) {
        return;
    }
    _uart->begin(serial_manager.find_baudrate(AP_SerialManager::SerialProtocol_PL_K, 0));
    _uart->set_stop_bits(2);
}

void PL_K::update()
{
    if (_last_message_ms != 0 && AP_HAL::millis() - _last_message_ms > 2000) {
        strncpy(_payload, "NO PAYLOAD", PAYLOAD_SIZE);
        _payload[PAYLOAD_SIZE] = '\0';
        _last_message_ms = 0;
    }

    if (_uart == nullptr) {
        return;
    }

    uint32_t available = _uart->available();
    while (available-- > 0) {
        const char c = _uart->read();

        if (c == '\0') {
            if (_line_len > 0) {
                _line_buf[_line_len] = '\0';
                parse_line(_line_buf);
                _line_len = 0;
            }
            continue;
        }

        if (_line_len < LINE_BUF_SIZE - 1) {
            _line_buf[_line_len++] = c;
        } else {
            // line too long — discard and reset
            _line_len = 0;
        }
    }
}

void PL_K::parse_line(const char *line)
{
    // TODO: parse ASCII message fields from 'line' and populate _payload
    strncpy(_payload, line, PAYLOAD_SIZE);
    _payload[PAYLOAD_SIZE] = '\0';
    _last_message_ms = AP_HAL::millis();
}

#endif  // AP_PL_K_ENABLED
