#include "test.hpp"
#include "joystick.hh"

//#include "can_linux_val.h"
//#include <stdio.h>
#include <cstdio>
int main(void)
{
    CAN_AVL can("can0");
    CAN_AVL can1("can0");
    can.write_param(0x156, 8);  // (id, dlc)
    can1.write_param(0x157,8);  // (id, dlc)

    int flag = 1;
    u_int16_t A_count = 0; //Alive Count

    // Create an instance of Joystick
    Joystick joystick("/dev/input/js0");

    // Ensure that it was found and that we can use it
    if (!joystick.isFound())
    {
        printf("open failed.\n");
        exit(1);
    }


    double target_steer = 0;
    double target_ACC = -2;
    u_int APM_EN = 0;
    u_int ASM_EN = 0;
    while(1)
    {
        JoystickEvent event;


        if (joystick.sample(&event))
        {
            if(event.isAxis())
            {
                if(event.number == 3)
                {
                    //target_steer = -500*(event.value/32767.);
                    //std::cout <<""<< event.value << std::endl;
                    if(abs(event.value) > 6000)
                        target_steer = -500*(event.value-6000)/(event.MAX_AXES_VALUE-6000);
                    else
                        target_steer = 0;
                    printf("joystick : %d target_steer : %f \n",event.value,target_steer);
                }
                if(event.number == 5)
                {
                    //target_steer = -500*(event.value/32767.);
                    //std::cout <<""<< event.value << std::endl;
                    target_ACC = event.value/32767.*5;

                    printf("joystick : %d target_acc : %f \n",event.value,target_ACC);
                }
            }
            if(event.isButton())
            {
                if(event.number == 4)
                {
                    event.value ? APM_EN=1:APM_EN=0 ;
                    printf ("APM : %d \n",APM_EN);
                }
                if(event.number == 5)
                {
                    event.value ? ASM_EN=2:ASM_EN=0 ;
                    printf ("ASM : %d \n",ASM_EN);
                }
            }
        }
        can.frame.data[0] = (APM_EN | 0x04);
        can.frame.data[1] = 150;
        can.frame.data[2] = ASM_EN;
        can.frame.data[7] = A_count++;
        flag = flag*(-1);
        can.nbytes = write(can.s, &can.frame, sizeof(struct can_frame));
      //  printf("frame[0]: %d \n",can.frame.data[0]);
        //        memset (can.frame.data,0,sizeof(__u8)*8);
        //   can.write();
        can1.frame.data[0] =((int32_t)(target_steer*10) & 0xff);
        can1.frame.data[1] =(((int32_t)(target_steer*10) & 0xff00)>>8);

        can1.frame.data[3] =((int32_t)(100*(target_ACC+10.23)) & 0xff);
        can1.frame.data[4] =(((int32_t)(100*(target_ACC+10.23)) & 0xff00)>>8);


        flag = flag*(-1);
        can1.nbytes = write(can1.s, &can1.frame, sizeof(struct can_frame));
        //can1.write();

        usleep(20000);
    }

    return 0;
}
