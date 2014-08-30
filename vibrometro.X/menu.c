#include<p33FJ64GP706.h>
#include "timer.h"
#include "i2c.h"
#include "lcd.h"
#include "menu.h"

#define CONFIRMA PORTGbits.RG15   //S1
#define AVANCA   PORTCbits.RC1    //S2
#define CANCELA  PORTCbits.RC2    //S3

int estado;

void menuprincipal(void)
{
    comandolcd(0x01);
    delay_us(50);
    escrevetexto("1x - Medidor");
    comandolcd(0xC0);              //2a linha
    escrevetexto("2x - Registrador");
    estado=0;
}

void modomedidor(void)
{
    comandolcd(0x01);
    delay_us(50);
    escrevetexto("Modo Medidor?");
}

void inicia_menu(void)
{
    //Configurações
    TRISGbits.TRISG15 = 1;         //Chave S1 como entrada
    TRISCbits.TRISC1 = 1;          //Chave S2 como entrada
    TRISCbits.TRISC2 = 1;          //Chave S3 como entrada
    AD1CON1bits.ADON = 0;          //Desabilita ADC compartilhado com S2 e S3
    AD1PCFGH = 0xFF;               //Seleciona modo digital nos pinos
    AD1PCFGL = 0xFF;

    inicia_lcd();                  //Inicializa display
    
    //Tela de apresentação
    escrevetexto("Medidor de");
    comandolcd(0xC0);              //2a linha
    escrevetexto("Vibracao");
    delay_ms(1500);                //1,5s
    comandolcd(0x01);              //Clear
    
    while(1)                          //Loop geral do menu
    {
       menuprincipal();               //Estado 0
    
       while(AVANCA != 1);
       modomedidor();                 //Estado 1
       delay_ms(1000);

       //Parte 1
       while((CONFIRMA||CANCELA) != 1)
       {
           while(AVANCA != 1)
           {
               if(CANCELA == 1)
               {
                   break;
               }
               else
               {
                   if(CONFIRMA == 1)
                   {
                       comandolcd(0x01);
                       delay_us(50);
                       escrevetexto("Modo Medidor!");
                       delay_ms(1000);
                       estado = 1;
                       while(CANCELA != 1)
                       {
                           le_sensor();
                       }
                       break;
                   }
               }
           }//fim do 2º avança

        //Inicio da Parte 2

        if(estado != 1)
        {
           comandolcd(0x01);
           delay_us(50);
           escrevetexto("Modo Registro!");
           delay_ms(800);

           while(CANCELA == 0)
           {
               comandolcd(0x01);
               delay_us(50);
               escrevetexto("Gravar no SD?");
               delay_ms(100);

               if(CONFIRMA != 0)
               {
                   comandolcd(0x01);
                   delay_us(50);
                   escrevetexto("Gravando...");
                   delay_ms(2500);
                   break;
               }


               //le_sensor();

           }

           //delay_ms(500); //contra trepidação
           break;

         }//fim do if de estado anterior

           
       }//fim while(CONFIRMA|CANCELA != 1) da parte 1

    }//fim while(1)

}//fim função inicia_menu