#include <iostream>
//#include "can_linux_val.h"
#include "test.hpp"

int main()
{
    std::cout <<" JY 1" << std::endl;

    int16_t steer_angle;
    int16_t steer_toq;
    int16_t Yaw_rate;
    int16_t LAT_ACCEL;
    CAN_AVL can("can0");
    CAN_AVL can1("can1");

    while(1)
    {
        // read(***) 만 써도 들어감. 다만 리턴값이 바이트를 출력하기 때문에 nbytes로 받을 수 있는 것.
        can.nbytes = read(can.s, &can.frame, sizeof(struct can_frame));
        can1.nbytes = read(can1.s, &can1.frame, sizeof(struct can_frame));
//	can.read();
//	can1.read();

        //printf("Wrote %d bytes,\t\t frame.data[7] = %d\n", nbytes, frame.data[7]);
        //  std::cout << "Wrote " << can.nbytes << " bytes, \t frame.data[7] = " << (int)can.frame.data[0] << "\tid = " << std::hex << can.frame.can_id << std::endl;

        if(can.frame.can_id==0x710)
        {
            //printf("Wrote %d bytes,\t\t frame.data[7] = %d\n", nbytes, frame.data[7]);
            //std::cout << "Wrote " << can.nbytes << " bytes, \t frame.data[7] = " << (int)can.frame.data[2] << "\tid = " << std::hex << can.frame.can_id << std::endl;

            steer_angle = can.frame.data[3]& 0xffff;
            steer_angle = steer_angle << 8;
            steer_angle = can.frame.data[2]|steer_angle;
          //  std::cout<<std::dec<< "핸들 각도 : "<< 0.1*steer_angle <<std::endl;

            steer_toq = can.frame.data[5] & 0x000f;
            steer_toq = steer_toq << 8;
            steer_toq = can.frame.data[4]|steer_toq;
        //    std::cout<<std::dec<< "핸들 토크 : "<< 0.01*(steer_toq-2048) <<std::endl;
          //  std::cout<<(int)can.frame.data[3]<<"   "<<(int)can.frame.data[2]<<std::endl;
        }
        else if(can1.frame.can_id==544)
        {
            Yaw_rate = can1.frame.data[6] & 0x1f;
            Yaw_rate = Yaw_rate << 8;
            Yaw_rate = can1.frame.data[5] | Yaw_rate;
        //    std::cout<<std::dec<<"Yaw rate :"<< 0.01 * Yaw_rate <<std::endl;

            LAT_ACCEL = can1.frame.data[1] & 0x7;
            LAT_ACCEL = LAT_ACCEL << 8;
            LAT_ACCEL = can.frame.data[0] | LAT_ACCEL;
          //  std::cout<<std::dec<<"LAT_ACCEL :"<< 0.01 * LAT_ACCEL <<std::endl;
        }

    }

	return 0;
}
