#include<p33FJ64GP706.h>

/* FOSC = 4MHz
 * Fcy=FOSC/2 = 2MHz, one instruction every two clock cycles
 * Tcy = 1/2.10E6 = 0.5 us
 * Período = PR1 x prescaler x Tcy
 * Cálculo em milisegundos: 1E-3 = PR1 x 1 x 0.5E-6 => PR1 = 2000
 */

int cont_tempo;                       //Contador auxiliar nos loops de tempo

//Função de atraso em ms
void delay_ms (int tempo_ms)
{
    T1CONbits.TON = 0;                //Desliga timer
    T1CONbits.TCS = 0;                //Clock source: Internal (Fosc/2)
    T1CONbits.TGATE = 0;              //Desabilita modo 'Gated'
    T1CONbits.TCKPS = 0;              //Prescale=1
    PR1 = 2000;                       //PR1 para 1ms
    

    for (cont_tempo=0;cont_tempo<=tempo_ms;cont_tempo++)
    {
        TMR1 = 0;                         //Reseta o registrador
        T1CONbits.TON = 1;                //Liga timer
        while(IFS0bits.T1IF==0);          //Aguarda o período
        IFS0bits.T1IF=0;                  //Limpa o flag
        T1CONbits.TON = 0;                //Desliga timer
    }


}


//Função de atraso em us
void delay_us (int tempo_us)
{
    T1CONbits.TON = 0;                //Desliga timer
    T1CONbits.TCS = 0;                //Clock source: Internal (Fosc/2)
    T1CONbits.TGATE = 0;              //Desabilita modo 'Gated'
    T1CONbits.TCKPS = 0;              //Prescale=1
    PR1 = 2;                          //PR1 para 1us


    for (cont_tempo=0;cont_tempo<=tempo_us;cont_tempo++)
    {
        TMR1 = 0;                         //Reseta o registrador
        T1CONbits.TON = 1;                //Liga timer
        while(IFS0bits.T1IF==0);          //Aguarda o período
        IFS0bits.T1IF=0;                  //Limpa o flag
        T1CONbits.TON = 0;                //Desliga timer
    }


}



