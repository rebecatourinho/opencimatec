#define FOSC 4000000                              //Frequência do oscilador
#define FCY (FOSC/2)                              //Frequência de operação
#define PGD 130E-9                                //Pulse Gobbler Delay
#define FSCL 100E3                                //Data transfer mode (Std)
#define BAUD ((1/FSCL-PGD)*FCY-2)                 //Cálculo do baud rate

#define ACK 0
#define NACK 1
#define SET 1
#define RESET 0
#define SF_FULL 0.0039                            // scale factor 3.9mg/LSB
#define SF_APROX 4                                  // scale factor aprox

//#define BAUD 0x11;                      //Baud rate, 100 KHz e XT=4MHz
//#define SET 1;
//#define RESET 0;

void config_comI2C1 (void);
void config_sensor (int registrador, int comando);
void num2char (signed int medida);
void le_sensor (void);