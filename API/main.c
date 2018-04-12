
/**
 * main.c
 * Author: AMAR SINGH
 *
 */
// K9F1G08U0E : NAND FLASH
// DRIVERS for  K9F1G08U0E
// DATA Pins are connected to K port of TM4C1294XL
// control pins are on Port D
//clock of controller was 16 MHZ....since timing is important


#include "NANDFlashDriver.h"
static volatile char arr1[NO_OF_BYTES_PER_PAGE];
//static volatile char arr3[NO_OF_BYTES_PER_PAGE];

//#define INVALID_BLOCKS
#ifdef INVALID_BLOCKS
static volatile char arr2[1024];
#endif //INVALID BLOCKS

void SomeDelay(int);

int main(void)
{
    volatile int i = 0;
    int temp1 = 0 ;
    int * temp1p = &temp1;
    int arr[NO_OF_BYTES_IN_READ_ID] = {0,0,0,0,0};
//    char arr1[NO_OF_BYTES_PER_PAGE];
//    char arr2[NO_OF_BYTES_PER_PAGE];



// Initialization of  Port-D for Control Signals

    RCGCGPIO = (1u<<9 | 1u<<3); // enabling clock to PORT K and PORT D
//    __asm("NOP");
    SomeDelay(1000);
    GPIODIR_D = ( ALE | CLE | WE | RE | CE ); //making Pins Output pin
    (*(GPIODATA_D + 0xF8u)) = ( CE | RE | WE ); // making WE RE & CE pins HIGH
    GPIODEN_D = ( ALE | CLE | WE | RE | CE | R_B );  //as soon as we digitally enable, above data shows up on the line
    GPIODIR_K = 0xFF; //making all DATA pins Output
    CE_CLR; //enabling the chip.....//no function has got control over it
// initialization complete.....we can call the expose functions below

    if(Reset())
        return 1;

    ReadID(arr);

// to extract the list of invalid blocks
#ifdef INVALID_BLOCKS
      for(i=0;i<1024;i++){
        temp1 = NO_OF_BYTES_PER_PAGE-1;
         do{
             arr1[temp1] = 0;
             temp1--;
         }while(temp1>=0);

       temp1 = ReadPage(arr1,ADDRESS(i,0)); //page 0
       arr2[i] = arr1[2048];
       temp1 = ReadPage(arr1,ADDRESS(i,1));//page 1
       arr2[i] = arr2[i]&arr1[2048];  //if a block is invalid than that index would have non 0xFF data
    }
#endif

//writing to block 1
    for(i=0;i<64;i++){
        temp1 = NO_OF_BYTES_PER_PAGE-1 ;
        do{
            arr1[temp1]= (char)i; //initialize the arr1 i.e. our page with page number as data
            temp1--;

        }while(temp1>=0);

        temp1 = WritePage(arr1,ADDRESS(1,i)); //write i-th page of 1st block
    }

//Verifying the above write to the 1st block
    for(i=0;i<64;i++){
        temp1 = ReadPage(arr1,ADDRESS(1,i)); //read the i-th page of 1st block

        temp1 = NO_OF_BYTES_PER_PAGE-1 ;
        do{
            if(!(arr1[temp1] == (char)i)) //check the validity of written data
                return 1; //data are not correct

            temp1--;

        }while(temp1>=0);


    }


	return 0;
}

void SomeDelay(int count){

    for(;count>0;count-- )
        __asm(" NOP ");

    return ;
}
