/*
 * NANDFlashDriver.c
 *
 *  Created on: 04-Mar-2018
 *      Author: AMAR SINGH
 */
#include "NANDFlashDriver.h"

int Reset(){
//This function Resets the NAND FLASH module

// default state code
    if(!READ_R_B)
        return 1;

    GPIODEN_K = 0; //making the lines high impedance

    //taking the control signal to base state i.e.
    CLE_CLR;
    ALE_CLR;
    RE_SET;
    WE_SET;

    if(!READ_R_B)
        return 1;

    GPIODIR_K = 0xFFU; //making all DATA pins Output
//default state code end

    (*GPIODATA_K) = 0xFFU;//making all pins High for Reset Command; data will get latched when GPIODEN Enabled

    CLE_SET;
    WE_CLR;
    GPIODEN_K = 0xFFU; //latching the data onto I/O Lines
    __asm(" NOP ");
    WE_SET;
    CLE_CLR;
    GPIODEN_K = 0; //making the lines high impedance

    while(!READ_R_B); //wait for Reset to complete

return 0;
}


int ReadID(int* const arr){
//This function sends the READ ID Command

        volatile unsigned int i = 0;
//default state code
        if(!READ_R_B)
            return 1;

        GPIODEN_K = 0; //making the lines high impedance

        //taking the control signal to base state i.e.
        CLE_CLR;
        ALE_CLR;
        RE_SET;
        WE_SET;

        if(!READ_R_B)
            return 1;

        GPIODIR_K = 0xFFU; //making all DATA pins Output
//default state code end

        (*GPIODATA_K) = 0x90U; // Read ID Command
        CLE_SET;
        WE_CLR;
        GPIODEN_K = 0xFFU; //latching the above data onto the line
        __asm(" NOP ");
        WE_SET;
        CLE_CLR;
        __asm (" NOP ");
        __asm(" NOP ");  //for CLE_CLR and ALE_SET delay
        ALE_SET;
        WE_CLR;
        (*GPIODATA_K) = 0x0; //  step 2 for Read ID command; latch address = 0
        __asm(" NOP ");
        WE_SET;
        ALE_CLR;
        GPIODEN_K = 0;
        GPIODIR_K = 0; // making input
        GPIODEN_K = 0xFFU; //enable


        while(i < NO_OF_BYTES_IN_READ_ID){
            RE_CLR;
            __asm(" NOP ");
            arr[i] = *(GPIODATA_K);
            RE_SET;

            i++;
        }

        return 0;
}

int WritePage(const char* const arr, unsigned int address ){
//this function write the "arr" array onto the page whose address is in "address"


    unsigned int i = 0;

//default state code
    if(!READ_R_B)
        return 1;

    GPIODEN_K = 0; //making the lines high impedance

    //taking the control signal to base state i.e.
    CLE_CLR;
    ALE_CLR;
    RE_SET;
    WE_SET;

    if(!READ_R_B)
        return 1;

    GPIODIR_K = 0xFFU; //making all DATA pins Output
//default state code end

// 0x80 command latch
    (*GPIODATA_K) = 0x80U; // Page Program Command start
    CLE_SET;
    WE_CLR;
    GPIODEN_K = 0xFFU; //latching the above data onto the line
    __asm(" NOP ");
    WE_SET;
    CLE_CLR;

//address latch
    __asm(" NOP ");
    __asm(" NOP ");
    ALE_SET;
    while(i<4){
        WE_CLR;
        (*GPIODATA_K) = (address >> (8*i)) & 0XFFU;
        __asm(" NOP ");
        WE_SET;

        i++;
    }
    ALE_CLR;
//address latch end

//Data Latch
    __asm(" NOP ");
    i = 0;
    while(i<NO_OF_BYTES_PER_PAGE){
       WE_CLR;
       (*GPIODATA_K) = arr[i];
       __asm(" NOP ");
       WE_SET;

       i++;
    }

//page program command latch-command
    __asm(" NOP ");
    CLE_SET;
    WE_CLR;
    (*GPIODATA_K) = 0x10U; // Page Program Command latch
    __asm(" NOP ");
    WE_SET;
    CLE_CLR;

    __asm(" NOP ");

    while(!READ_R_B);// waiting for write to complete
//TODO: detect the write fail; perform the Reset and return the corresponding errors

//taking things to base state
    GPIODEN_K = 0; //making the lines high impedance again
    //GPIO are already output
    //already: CLE_CLR;ALE_CLR;WE_SET;RE_SET;

return 0;
} //WritePage end

int ReadPage(char* const arr,int address){
//this function reads a page at "address" into "arr"

    unsigned int i = 0;

//default state code
    if(!READ_R_B)
        return 1;

    GPIODEN_K = 0; //making the lines high impedance

    //taking the control signal to base state i.e.
    CLE_CLR;
    ALE_CLR;
    RE_SET;
    WE_SET;

    if(!READ_R_B)
        return 1;

    GPIODIR_K = 0xFFU; //making all DATA pins Output
//default state code end

// 0x00 command latch
    (*GPIODATA_K) = 0x00; // Page Read Command start
    CLE_SET;
    WE_CLR;
    GPIODEN_K = 0xFFU; //latching the above command onto the line
    __asm(" NOP ");
    WE_SET;
    CLE_CLR;

//address latch

    __asm(" NOP ");
    __asm(" NOP ");
    ALE_SET;
    while(i<4){
        WE_CLR;
        (*GPIODATA_K) = (address >> (8*i)) & 0XFFU;
        __asm(" NOP ");
        WE_SET;

        i++;
    }
    ALE_CLR;
//address latch end

    __asm(" NOP ");
    __asm(" NOP ");
    CLE_SET;
    WE_CLR;
    (*GPIODATA_K) = 0x30; // Page Read Command latch
    __asm(" NOP ");
    WE_SET;
    CLE_CLR;

    GPIODEN_K = 0;
    GPIODIR_K = 0; // making input
    GPIODEN_K = 0xFF;

    while(!READ_R_B); //wait till "tR"

    __asm(" NOP ");
    __asm(" NOP ");

    i = 0;
    while(i<NO_OF_BYTES_PER_PAGE){
        RE_CLR;
        __asm(" NOP ");
        arr[i] = *(GPIODATA_K);
        RE_SET;

        i++;
    }

    GPIODEN_K = 0;
    GPIODIR_K = 0xFF; // making output
    //already: CLE_CLR;ALE_CLR;WE_SET;RE_SET;

return  0;
}
