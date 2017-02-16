#include "Copter.h"

#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
    //printf("1hz loop called    \n");

        Location loc;
        if (ahrs.get_position(loc)) {
            char buf[100];
            ::sprintf(buf,"lat:%ld lon:%ld alt:%ld\n",
                    (long)loc.lat,
                    (long)loc.lng,
                    (long)loc.alt);

            telemetry.send_text(buf);
        }
    char cmd[100];
    mavlink_message_t msg;

    if (telemetry.recv_command(cmd) != 0)
    {

        printf("received command from Pc %s \n", cmd);
        if(strncmp(cmd, "arm", 3) == 0)
        {
            //arm コマンド発行
            memset(&msg, 0, sizeof(msg));
            msg.msgid = MAVLINK_MSG_ID_COMMAND_LONG;
            mavlink_msg_command_long_pack_chan(
                0,0,0,
                &msg,
                0,0,MAV_CMD_COMPONENT_ARM_DISARM,0,
                1.0,0.0,0.0,0.0,0.0,0.0,0.0);
            gcs[0].handleMessage(&msg);
        } else if (strncmp(cmd, "takeoff", 7) == 0){
            float takeoff_alt = 20;// param7
            float hnbpa = 1.0; // param 3 horizontal navigation by pilot acceptable
            memset(&msg, 0, sizeof(msg));
            msg.msgid = MAVLINK_MSG_ID_COMMAND_LONG;
            mavlink_msg_command_long_pack_chan(
                0,0,0,
                &msg,
                0,0,MAV_CMD_NAV_TAKEOFF,0,
                0.0,0.0,hnbpa,0.0,0.0,0.0,takeoff_alt);
            gcs[0].handleMessage(&msg);



        }
    }



}
#endif
