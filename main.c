

/**
 * main.c
 */
// K9F1G08U0E : NAND FLASH
// THIS code do the reset and reads the ID of above IC
// DATA Pins are connected to K port of TM4C1294XL
// control pins are on Port D
//clock of controller was 16 MHZ....since timing is important
#include "NANDFlashDriver.h"

void SomeDelay(volatile unsigned int);

int main(void)
{
    int temp1 = 0 ;
    int * temp1p = &temp1;
    int arr[5] = {0,0,0,0,0};

// Initialization of  Port-D for Control Signals

    RCGCGPIO = (1u<<9 | 1u<<3); // enabling clock to PORT K and PORT D
    __asm(" NOP ");
    SomeDelay(1000);
    GPIODIR_D = ( ALE | CLE | WE | RE | CE ); //making Pins Output pin
    (*(GPIODATA_D + 0xF8u)) = ( CE | RE | WE ); // making WE RE & CE pins HIGH
    GPIODEN_D = ( ALE | CLE | WE | RE | CE | R_B );  //as soon as we digitally enable, above data shows up on the line
    GPIODIR_K = 0xFF; //making all DATA pins Output


// RESET COMMAND
//    if(READ_R_B)
    (*GPIODATA_K) = 0xFF;//making all pins High for Reset Command

    CE_CLR;
    CLE_SET;
    WE_CLR;
    GPIODEN_K = 0xFF; //latching the data onto I/O Lines
    WE_SET;
    CLE_CLR;
    GPIODEN_K = 0; //making the lines high impedance

    while(!READ_R_B)
        (*temp1p)+=1; // avoiding optimization of temp1 variable with the pointer aliasing

//  READ ID COMMAND
    (*GPIODATA_K) = 0x90; // Read ID Command
    CLE_SET;
    WE_CLR;
    GPIODEN_K = 0xFF; //latching the above data onto the line
    WE_SET;
    CLE_CLR;
    GPIODEN_K = 0;
    __asm(" NOP ");
    (*GPIODATA_K) = 0x0; //  step 2 for Read ID command; latch address = 0
    ALE_SET;
    WE_CLR;
    GPIODEN_K = 0xFF;
    WE_SET;
    ALE_CLR;
    GPIODEN_K = 0;
    GPIODIR_K = 0; // making input
    GPIODEN_K = 0xFF;

    RE_CLR;
    arr[0] = *GPIODATA_K; //0xEC
    RE_SET;
    __asm(" NOP ");

        RE_CLR;
        arr[1] = *GPIODATA_K;//0xF1
        RE_SET;
        __asm(" NOP ");

            RE_CLR;
            arr[2] = *GPIODATA_K;//00
            RE_SET;
            __asm(" NOP ");

                RE_CLR;
                arr[3] = *GPIODATA_K;//0x95
                RE_SET;
                __asm(" NOP ");

                                RE_CLR;
                                arr[4] = *GPIODATA_K;//0x41
                                RE_SET;


      *temp1p = arr[0]+arr[1]+arr[2]+arr[3]+arr[4];

	return 0;
}

void SomeDelay(volatile unsigned int count){

    for(;count>0;count-- )
        __asm(" NOP ");

}
