#include <p33FJ64GP706.h>
#include "lcd.h"
#include "timer.h"

#define RS PORTDbits.RD8
#define RW PORTDbits.RD9
#define ENABLE PORTDbits.RD10

/* COMANDOS BÁSICOS DO LCD
 * Cursor home = 0x02 - Requer 2ms
 * Clear = 0x01 - requer 2ms
 * Cursor piscante = 0x0D;
 * 1a linha = 0x80;
 * 2a linha = 0xC0;
 */

//Função que envia um comando para o display
void comandolcd(char comando_lcd)
{
    PORTD = comando_lcd;
    //RS = 0;
    PORTDbits.RD8 = 0;          //Habilita envio de COMANDOS
    delay_us(50);
    //delay_us(100);
    //delay_us(500);
    //delay_ms(1);
    //RW = 0;                   //Modo escrita
    PORTDbits.RD9 = 0;
    //ENABLE = 1;               //Enable, borda de descida
    PORTDbits.RD10 = 1;
    delay_us(50);
    //delay_us(100);
    //delay_us(500);
    //delay_ms(1);
    //ENABLE = 0;
    PORTDbits.RD10 = 0;         //Desabilita Enable
    delay_us(39);
}

//Função que envia um caractere (dado) para o display
void escrchar(char caractere)
{
    PORTD = caractere;
    //RS = 1;                  
    PORTDbits.RD8 = 1;         //Habilita envio de DADOS
    delay_us(50);
    //delay_us(100);
    //delay_us(500);
    //delay_ms(1);
    //RW = 0;                  //Modo escrita
    PORTDbits.RD9 = 0;         
    //ENABLE = 1;              //Enable, borda de descida
    PORTDbits.RD10 = 1;
    delay_us(50);
    //delay_us(100);
    //delay_us(500);
    //delay_ms(1);
    //ENABLE = 0;
    PORTDbits.RD10 = 0;        //Desabilita Enable
    delay_us(39);
}

//Função que inicializa o display
void inicia_lcd(void)
{
    TRISD = 0;          //Configura porta D como saída
    delay_ms(30);
    comandolcd(0x38);   //Function set: 8bit, 2 linhas, 5x8 dots
    delay_ms(15);
    comandolcd(0x38);   //É necessário?!
    delay_ms(15);
    comandolcd(0x06);   //Cursor para a direita
    delay_ms(50);
    comandolcd(0x0C);   //Liga display sem cursor
    delay_ms(15);
    comandolcd(0x01);   //Limpa display
    delay_ms(17);
}

/*
void escrtexto (char texto[16])
{
    int cont_char;
    for (cont_char=0;cont_char<16;cont_char++){
        escrchar(texto[cont_char]);
    }
}
*/

/* Função que escreve uma sequência de caracteres de tamanho qualquer
 * e que é interrompida ao primeiro caractere igual a 0x00 encontrado */
void escrevetexto(char *texto)
{
    while(*texto)
    {
        escrchar(*texto);
        *texto++;
    }
}