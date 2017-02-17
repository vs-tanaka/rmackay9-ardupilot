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

#include "AP_Telemetry_MQTT.h"
#include <stdio.h>
#include "../../modules/Mqtt/MQTTAsync.h"
extern const AP_HAL::HAL& hal;

extern void start_send(char *buf);
extern int finished;
extern int connected;

extern int disc_finished;
extern int sub_connect_stat;
extern int subscribed;
extern int finished_sub;

extern MQTTAsync client;
extern int start_subscribe(void);
extern void *start_connect();
extern int recv_data(char *str);

extern void start_send_text(void* context, const char *str);
extern int sub_connect_stat;
extern void init_subscribe();

AP_Telemetry_MQTT::AP_Telemetry_MQTT(AP_Telemetry &frontend, AP_HAL::UARTDriver* uart) :
        AP_Telemetry_Backend(frontend, uart)
{

    printf("AP_Telemetry_MQTT");
    init_subscribe();

}



void AP_Telemetry_MQTT::send_text(const char *str) 
{
    
    if((connected == 1) && (finished == 0) && (stage == 2) && (client != NULL))
    {
        start_send_text(client, str);
    }


}


int AP_Telemetry_MQTT::recv_message(char *str) 
{
    int ret;
    ret = 0;
    if((stage_sub == 1) && (sub_connect_stat == 2) && (finished_sub == 0) &&
       (disc_finished == 0))
    {
        ret = recv_data(str);
    }
  return ret;

}


// update - provide an opportunity to read/send telemetry
void AP_Telemetry_MQTT::update()
{
    // exit immediately if no uart
    if (_uart == nullptr || _frontend._ahrs == nullptr) {
   //     return;
    }

    // send telemetry data once per second
    uint32_t now = AP_HAL::millis();
    if (_last_send_ms == 0 || (now - _last_send_ms) > 1000) {
        _last_send_ms = now;
        Location loc;
        if (_frontend._ahrs->get_position(loc)) {
            char buf[100];
            ::sprintf(buf,"lat:%ld lon:%ld alt:%ld\n",
                    (long)loc.lat,
                    (long)loc.lng,
                    (long)loc.alt);
            
            switch (stage)
            {
                case 0://stage disconnect
                    client = start_connect();
                    if(client != NULL)
                    {
                        stage = 1;// waiting for connection finish
                    }
                    break;
                case 1:
                    if (connected == 1)
                    {
                        stage = 2;
                    }
                    break;
                case 2:
                    if(finished == 1)
                    {
                        MQTTAsync_destroy(&client);
                        stage = 3;
                        connect_timer = 10;
                    }
                    break;
                case 3:
                    if(connect_timer > 0)
                    {
                        connect_timer--;
                    } else {
                        stage = 0;
                    }
                    break;
               
            }
            switch (stage_sub)
            {
                case 0:
                    if (sub_connect_stat == 0)
                    {
                        start_subscribe();
                        stage_sub = 1;
                        
                    }
                    break;
                case 1:
                    if((finished_sub == 1) || (disc_finished == 1))
                    {
                        connect_timer_sub = 10;
                        stage_sub = 2;
                    }
                    break;
                case 2:
                    if(connect_timer_sub > 0)
                    {
                        connect_timer_sub--;
                    } else {
                        sub_connect_stat = 0;
                        stage_sub = 0;
                    }
                    break;
                    
            }

        }
    }
}
