/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <AP_HAL/AP_HAL.h>
#include <AP_AHRS/AP_AHRS.h>
#include <AP_SerialManager/AP_SerialManager.h>
#include "AP_Telemetry_Backend.h"
    //static inline void byte_copy_4(char *dst, const char *src)
   // {
//	dst[0] = src[0];
//	dst[1] = src[1];
//	dst[2] = src[2];
//	dst[3] = src[3];
  //  }

    //#define _mav_put_uint8_t(buf, wire_offset, b) buf[wire_offset] = (uint8_t)b
    //#define _mav_put_uint32_t(buf, wire_offset, b) byte_copy_4(&buf[wire_offset], (const char *)&b)

class AP_Telemetry_MQTT : public AP_Telemetry_Backend
{
public:
    int stage = 0;
    int stage_sub = 0;
    void *client;
    int connect_timer = 0;
    int connect_timer_sub = 0;
    AP_Telemetry_MQTT(AP_Telemetry &frontend, AP_HAL::UARTDriver* uart);

    // update - provide an opportunity to read/send telemetry
    void update() override;
    void send_text(const char *str) override;
    int recv_mavlink_message(mavlink_message_t *msg) override;
    
 


private:

    uint32_t _last_send_ms;
};
