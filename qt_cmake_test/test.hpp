
#ifndef CAN_AVL_H
#define CAN_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// 리눅스 내장 Socket 헤더
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

// 리눅스 내장 Socket 헤더
#include <linux/can.h>
#include <linux/can/raw.h>

#include <iostream>
#include <string.h>

class CAN_AVL
{
public:
    int s;
    int nbytes;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    // 장치 이름이 can0인지 확인하기 위해 선언
    const char *ifname;

    CAN_AVL(const char *dev_name_);
    void CAN_INIT(const char *dev_name_);
    void write_param(int id_, int dlc_);
    ssize_t can_write();
    ssize_t can_read();
};

ssize_t CAN_AVL::can_write()
{
    int nbytes_tmp;
    nbytes_tmp = write(s, &frame, sizeof(struct can_frame));
    nbytes = nbytes_tmp;
    return nbytes_tmp;
}

ssize_t CAN_AVL::can_read()
{
    int nbytes_tmp;
    nbytes_tmp = read(s, &frame, sizeof(struct can_frame));
    nbytes = nbytes_tmp;
    return nbytes_tmp;
}
CAN_AVL::CAN_AVL(const char *dev_name_)
{
    CAN_INIT(dev_name_);
}

void CAN_AVL::CAN_INIT(const char *dev_name_)
{
    std::cout << " CAN_AVL class Constructor \t: opened" << std::endl;
    ifname = dev_name_;
    std::cout << " DEV_NAME \t\t\t: " << ifname << std::endl;

    // 소켓이 열렸는지 확인
    if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket");
        //return -1;
    }
    else std::cout << " Socket State \t\t: opened " << std::endl;

    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    printf(" Index \t\t\t: %d\n", ifr.ifr_ifindex);

    // 소켓 bind 확인
    if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        //return -2;
    }
}
void CAN_AVL::write_param(int id_, int dlc_)
{
    frame.can_id = id_;
    frame.can_dlc = dlc_;
}

#endif // CAN_AVL_H
