# Lumenier LUX H743 V2 Flight Controller - NDAA

The Lumenier LUX H743 V2 NDAA flight controller is sold by
[GetFPV](https://www.getfpv.com/).

## Features

- Processor: MCU STM32H743, 480MHz, 1MB RAM, 2MB Flash
- Dual LSM6DSK320X IMUs
- DPS310 Barometer
- IIS2MDC Compass
- microSD Card Slot
- 12 PWM Outputs
- I2C Port for External Sensors
- CANbus Support
- 7 UART Ports
- LED Strip Control
- Analog Current Sensor Input
- Analog Battery Sensor Input
- Analog RSSI Input
- Analog Airspeed Input
- USB Type-C (2.0)
- Blackbox Storage - SD Card and Flash (128Mbit/16Mbyte)
- Camera Control Output
- Camera Switch Output
- VTX Power Switch Output
- Supported Firmware - Betaflight and ArduPilot
- NDAA compliant

## UART Mapping

- SERIAL0 -> USB
- SERIAL1 -> USART2 (Telem1, DMA enabled, RTS/CTS)
- SERIAL2 -> USART3 (Telem2, DMA enabled, RTS/CTS)
- SERIAL3 -> USART1 (GPS1, DMA enabled)
- SERIAL4 -> UART5  (GPS2)
- SERIAL5 -> UART4  (DisplayPort)
- SERIAL6 -> UART7  (RCinput, DMA enabled)
- SERIAL7 -> UART8  (ESC Telemetry)

## SPI Mapping

| SPI | Device |
| --- | --- |
| 1 | W25Q128 Flash |
| 2 | SD Card |
| 3 | LSM6DSK320X IMU 1 |
| 4 | LSM6DSK320X IMU 2 |

## I2C Mapping

- I2C1 -> Internal (DPS310 barometer, IIS2MDC compass)
- I2C4 -> External

## PWM Output

The Lumenier LUX H743 V2 NDAA supports 12 PWM outputs and a serial LED PWM output.
All outputs are DShot capable. Outputs 1, 3, 5 and 7 are bi-directional DShot capable.
The 8 main PWM outputs are labeled M1 through M8.
The 4 auxiliary outputs are labeled S1 through S4.

The 8 main PWM outputs are in 2 groups:

- PWM 1 through 4 (M1 - M4) are in TIM1
- PWM 5 through 8 (M5 - M8) are in TIM8

The auxiliary PWM outputs are grouped as follows:

- PWM 9 through 12 (S1 - S4) are in TIM4
- PWM 13 (LED) is in TIM3

## RC Input

Bi-directional RC inputs like CRSF/ELRS are supported on UART7 (Serial6).
UART7 (Serial6) will also support all unidirectional RC protocols.
PPM input is supported on PA2.

## OSD Support

The LUX H743 V2 - NDAA does not have an onboard analog OSD. MSP DisplayPort
OSD is enabled by default on UART4 (Serial5).

## Camera and VTX Control

The LUX H743 V2 - NDAA has a camera switch output on PB1 (GPIO 81) and a VTX
power switch output on PB2 (GPIO 82). RELAY1 is pre-configured to control
GPIO 81 and RELAY2 is pre-configured to control GPIO 82.

## Analog inputs

The LUX H743 V2 NDAA has 4 analog inputs:

- PC0 -> Battery Voltage
- PC1 -> Battery Current
- PC4 -> Airspeed
- PC5 -> RSSI

## Battery Monitor

The LUX H743 V2 - NDAA has an internal voltage sensor and connections on the ESC
connector for an external current sensor input.

The default parameters are as follows:

- :ref:`BATT_MONITOR<BATT_MONITOR>` = 4
- :ref:`BATT_VOLT_PIN<BATT_VOLT_PIN__AP_BattMonitor_Analog>` = 10
- :ref:`BATT_CURR_PIN<BATT_CURR_PIN__AP_BattMonitor_Analog>` = 11
- :ref:`BATT_VOLT_MULT<BATT_VOLT_MULT__AP_BattMonitor_Analog>` = 10.1
- :ref:`BATT_AMP_PERVLT<BATT_AMP_PERVLT__AP_BattMonitor_Analog>` = 17.0 (will need to be adjusted for whichever current sensor is attached)

## Compass

The LUX H743 V2 - NDAA has a built-in IIS2MDC compass. An external compass can
also be attached using I2C on the SDA and SCL pads.

## Firmware

Firmware for the LUX H743 V2 - NDAA can be found [here](https://firmware.ardupilot.org) in sub-folders labeled "LumenierLUXH743V2-NDAA".

## Loading Firmware

The LUX H743 V2 - NDAA does not come with ArduPilot firmware pre-installed. Use instructions here to load ArduPilot the first time :ref:`common-loading-firmware-onto-chibios-only-boards`.

Initial firmware load can be done with DFU by plugging in USB with the
bootloader button pressed. Then you should load the "with_bl.hex"
firmware, using your favorite DFU loading tool.

Once the initial firmware is loaded you can update the firmware using
any ArduPilot ground station software. Updates should be done with the
*.apj firmware files.
