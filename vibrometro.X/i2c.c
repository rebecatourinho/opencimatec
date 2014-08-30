#include<p33FJ64GP706.h>
#include "i2c.h"
#include "timer.h"
#include "lcd.h"
#define TAMANHO 512


int cont_data;                 //Contador para acessar e ordenar dados
int endr_ini;                  //Variável que contém endereço inicial
char dados[6];                 //Vetor para armazenar DATA X0/X1/Y0/Y1/Z0/Z1
int cont_conc,cont_eixo;       //Contadores auxiliares
signed int eixoX[TAMANHO];          //Vetor de medições concatenadas em X
signed int eixoY[TAMANHO];          //Vetor de medições concatenadas em Y
signed int eixoZ[TAMANHO];          //Vetor de medições concatenadas em Z
signed int eixoZ_aux;
signed int eixoY_aux;
signed int eixoX_aux;
char medXYZ[7];                //Medida de 5 dígitos + '0' + sinal


//Configuração do I2C1
void config_comI2C1 (void)
{
    ODCGbits.ODCG2=1;          //Configura SCL1/RG2 como Open Drain
    ODCGbits.ODCG3=1;          //Configura SDA1/RG3 como Open Drain
    I2C1CONbits.I2CEN = 1;     //Habilita o módulo I2C do dsPIC
    I2C1BRG = BAUD;            //Configura baud rate de 100 KHz para XT=4MHz
    IEC1bits.MI2C1IE = 1;      //Habilita a interrupção do master
    IEC1bits.SI2C1IE = 1;      //Habilita a interrupção do slave
}


//Comunicação como mestre em um sistema de único mestre
void config_sensor (int registrador, int comando)
{
    for (cont_data=0;cont_data<6;cont_data++)
    {

        I2C1CONbits.SEN = 1;                    //Start
        while(I2C1CONbits.SEN == 1);

        I2C1TRN = 0xA6;                         //Slave Address + Write=0
        while(I2C1STATbits.TRSTAT == 1);

        I2C1TRN = registrador;                  //Endereço do registrador
        while(I2C1STATbits.TRSTAT == 1);

        I2C1TRN = comando;                      //Comando
        while(I2C1STATbits.TRSTAT == 1);

        I2C1CONbits.PEN = 1;                    //Stop
        while(I2C1CONbits.PEN == 1);

    }

}

//Função que converte medida em ASCII com sinal +/-
void num2char (signed int medida)
{
    medXYZ[6]=0;                               //Caractere de parada
    int cont_char=5;                           //Num máximo de algarismos, 16g
    medXYZ[0]='+';                             //Sinal no display

    if (medida<0){
        medida = medida*(-1);                  //Obtém valor positivo (absoluto)
        medXYZ[0]='-';                         //Altera o sinal para negativo
    }

    while(cont_char>0){
        medXYZ[cont_char] = medida%10 + 0x30;  //Resto da divisão + ASCII
        medida = medida/10;                    //Remove uma dezena
        cont_char--;
    }

}


//Função que realiza a leitura de uma medida XYZ e exibe no display
void le_sensor (void)
{
    for (cont_data=0;cont_data<6;cont_data++)
    {
        
        I2C1CONbits.SEN = 1;                    //Start condition
        while(I2C1CONbits.SEN == 1);
        //I2C1STATbits.BCL = 0;
        I2C1TRN = 0xA6;                         //Slave Address + Write=0

        while(I2C1STATbits.TRSTAT == 1);

        endr_ini = 0x32;                        //Endereço inicial, DATA_X0
        I2C1TRN = endr_ini + cont_data;
        while(I2C1STATbits.TRSTAT == 1);

        I2C1CONbits.RSEN = 1;                   //Repeated start
        while(I2C1CONbits.RSEN == 1);
        
        I2C1TRN = 0xA7;                         //Address + Read=1
        while(I2C1STATbits.TRSTAT == 1);
        
        I2C1CONbits.RCEN = 1;                   //Habilita recepção
        while(I2C1CONbits.RCEN == 1);
        dados[cont_data] = I2C1RCV;             //Armazena dado recebido

        I2C1CONbits.ACKDT = NACK;               //Envia NACK
        I2C1CONbits.ACKEN = 1;
        while(I2C1CONbits.ACKEN == 1);
        I2C1CONbits.PEN = 1;                    //Stop condition
        while(I2C1CONbits.PEN == 1);
        
    }

    //Concatena medida de 16 bits em X e converte para mg
    eixoX[cont_eixo] = dados[1];
    eixoX[cont_eixo] = eixoX[cont_eixo] << 8;
    eixoX_aux=dados[0]&0x00FF;
    eixoX[cont_eixo] = (eixoX[cont_eixo]|eixoX_aux);
    eixoX[cont_eixo] = eixoX[cont_eixo]*SF_APROX;

    num2char(eixoX[cont_eixo]);
    comandolcd(0x80);               //1a linha
    escrevetexto("X");
    escrevetexto(medXYZ);

    //Concatena medida de 16 bits em Y
    eixoY[cont_eixo] = dados[3];
    eixoY[cont_eixo] = eixoY[cont_eixo] << 8;
    eixoY_aux=dados[2]&0x00FF;
    eixoY[cont_eixo] = (eixoY[cont_eixo]|eixoY_aux);
    eixoY[cont_eixo] = eixoY[cont_eixo]*SF_APROX;
    
    num2char(eixoY[cont_eixo]);
    escrevetexto(" Y");
    escrevetexto(medXYZ);

    //Concatena medida de 16 bits em Z
    eixoZ[cont_eixo] = dados[5];
    eixoZ[cont_eixo] = eixoZ[cont_eixo] << 8;
    eixoZ_aux=dados[4]&0x00FF;
    eixoZ[cont_eixo] = (eixoZ[cont_eixo]|eixoZ_aux);
    eixoZ[cont_eixo] = eixoZ[cont_eixo]*SF_APROX;

    comandolcd(0xC0);               //2a linha
    num2char(eixoZ[cont_eixo]);
    escrevetexto("Z");
    escrevetexto(medXYZ);

    //Incrementa o contador a cada leitura
    cont_eixo++;

    //Limpa contador após vetor ser preenchido completamente
    if (cont_eixo >= TAMANHO){
        cont_eixo = 0;
    }


}


//Qual a necessidade de implementar essas interrupções?
//Master Events
void __attribute__((__interrupt__,__auto_psv__))  _MI2C1Interrupt(void)
{
   
   IFS1bits.MI2C1IF = 0;            //Limpa MI2CxIF
   if(I2C1STATbits.RBF==1)          //RBF é zerado após CPU ler buffer
   {
       //cont_data=cont_data;       //Não sei o porquê desta linha
   }
   
}

//Slave Events
void __attribute__((__interrupt__,__auto_psv__))  _SI2C1Interrupt(void)
{
   IFS1bits.SI2C1IF = 0;
}



 /* Complemento de 2
  *
    if ((eixoX[cont_eixo] & 0xF000) == 0xF000){
        eixoX[cont_eixo] = (~(eixoX[cont_eixo] + 1))*(-1);
    }
    */