/**
  ********************************************************************************
  * @File Name    : stm32Hardware.h
  * @Author       : Jungle
  * @Mail         : Mail
  * @Created Time : 2018/7/16 17:29:18
  * @Version      : V1.0
  * @Last Changed : 2018/7/16 17:29:18
  * @Brief        : brief
  ********************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __STM32HARDWARE_H_
#define __STM32HARDWARE_H_

#include "stdio.h"

extern "C" {
#include <bsp.h>
}

class Stm32Hardware {
public:
    Stm32Hardware();

    void init(void);

    int read(void);

    void write(unsigned char* data, int length);

    unsigned long time(void);
private:

};

#endif