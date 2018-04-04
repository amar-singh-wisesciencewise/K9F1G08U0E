/*
 * NANDFlashDriver.h
 *
 *  Created on: 04-Mar-2018
 *      Author: amar
 */

#ifndef NANDFLASHDRIVER_H_
#define NANDFLASHDRIVER_H_
// CLOCK GATE ENABLE

#define RCGCGPIO (*(volatile unsigned int*)(0x400FE000+0x608)) //to enable the clock to the GPIO port// bit 0 for PORT A and bit 3 for PORT D


#define GPIO_K_BASE 0x40061000u  ///port K base address
#define GPIODIR_K (*(volatile unsigned int*)(GPIO_K_BASE + 0x400))
#define GPIODATA_K ((volatile unsigned int*)(GPIO_K_BASE + 0x3FC))
#define GPIODEN_K (*(volatile unsigned int*)(GPIO_K_BASE + 0x51C))
#define GPIOSLR_K (*(volatile unsigned int*)(GPIO_K_BASE + 0x518))

#define GPIO_D_BASE 0x4005B000u
#define GPIODIR_D (*(volatile unsigned int*)(GPIO_D_BASE + 0x400))
#define GPIODATA_D ((volatile unsigned int*)(GPIO_D_BASE + 0x000))
#define GPIODEN_D (*(volatile unsigned int*)(GPIO_D_BASE + 0x51C))
#define GPIOSLR_D (*(volatile unsigned int*)(GPIO_D_BASE + 0x518))

#define R_B (1U<<0)  // D[0] input
#define ALE (1U<<1)  //D[1]  output
#define CLE (1U<<2)  //D[2]  output
#define WE (1U<<3)   //D[3]  output
#define RE (1U<<4)  // D[4]  output
#define CE (1U<<5)  //D[5]   output


#define ALE_SET  GPIODATA_D[ALE]=ALE   // *(0x4005B000 + 2 * 4) = 2
#define ALE_CLR  GPIODATA_D[ALE]=0
#define CLE_SET  GPIODATA_D[CLE]=CLE   // *(0x4005B000 + 4 * 4) = 4
#define CLE_CLR  GPIODATA_D[CLE]=0
#define WE_SET  GPIODATA_D[WE]=WE
#define WE_CLR  GPIODATA_D[WE]=0
#define RE_SET  GPIODATA_D[RE]=RE
#define RE_CLR  GPIODATA_D[RE]=0
#define CE_SET  GPIODATA_D[CE]=CE
#define CE_CLR  GPIODATA_D[CE]=0
#define READ_R_B  GPIODATA_D[R_B]



#endif /* NANDFLASHDRIVER_H_ */
