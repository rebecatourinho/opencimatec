/* File:   main.c
 * Author: rebecatourinho
 * Created on March 24, 2014, 10:29 AM
 * Modified on May 23, 2014, 09:55 PM
 */

#include<p33FJ64GP706.h>
#include "timer.h"
#include "i2c.h"
#include "lcd.h"
#include "menu.h"

//Configurations Bits
//Flash Boot Segmentation
_FBS(BWRP_WRPROTECT_OFF & BSS_NO_BOOT_CODE & RBS_NO_BOOT_RAM);  //default 0xFFFF
//Flash Secure Segmentation
_FSS(SWRP_WRPROTECT_OFF & SSS_NO_SEC_CODE & RSS_NO_SEC_RAM);    //default 0xFFFF
//General Segmentation
_FGS(GWRP_OFF & GSS_OFF);                                      //default 0x/FFFF
//Oscillator source and start-up
_FOSCSEL(FNOSC_PRI & IESO_ON);                 //Primary (XT, HS, EC) oscillator
//Oscillator Mode
_FOSC(POSCMD_XT & OSCIOFNC_OFF & FCKSM_CSDCMD);             //XT Oscillator Mode
//Watchdog timer
_FWDT(WDTPOST_PS1 & WDTPRE_PR32 & WINDIS_OFF & FWDTEN_OFF);            //WTD off
//Power-up timer delay time
_FPOR(FPWRT_PWR128);                                           //default 0x/FFFF
//ICD Communication Channel
_FICD(ICS_PGD1 & JTAGEN_OFF);         //Communicate on PGC1/EMUC1 and PGD1/EMUD1

//Vetores de medições XYZ
/*extern signed int eixoX[32];
extern signed int eixoY[32];
extern signed int eixoZ[32];
*/


int main(void)
{

    //Configura e habilita comunicação I2C1
    config_comI2C1();
    
    //Sequência de configuração do ADXL345: Registrador e comando
    config_sensor(0x2C,0x0F);      //Data rate=3200Hz BW=1600Hz, normal power
    config_sensor(0x31,0x0B);      //Full-bit, 16g, 3.9mg/LSB
    //config_sensor(0x31,0x08);    //Full, 2g
    //config_sensor(0x31,0x09);    //Full, 4g
    config_sensor(0x2D,0x08);      //Modo measurement


    //inicia_lcd();
    inicia_menu();
    
/*
    while(1)
    {
        le_sensor();                //Amostra e exibe na tela
        //delay_ms(80);              //taxa de amostragem
      
    }
*/
 
    
}

