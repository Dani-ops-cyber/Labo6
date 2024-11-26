////---------------------------------LABO6
//EJERCICIO 5
// In TIVA Receive the duty cycle of the Raspberry Pi and create a
//sequence with Timers. When the duty cycle is below 50% use Timer
//1 and blink an external LED every second, when de duty cycle is
//above 50% blink the LED every half of a second.

// %%%%%%%%%%%%%%%%%%%%%%%% IMPORTAMOS LIBRERIAS %%%%%%%%%%%%%%%%%%%%%%%%
#include <stdint.h>   
#include "stdio.h"    
#include "stdlib.h"   
#include <stdbool.h>  
#include <string.h>   
#include "inc/hw_memmap.h"  
#include "driverlib/debug.h" 
#include "driverlib/gpio.h"  
#include "driverlib/interrupt.h" 
#include "driverlib/pin_map.h"   
#include "driverlib/sysctl.h"    
#include "driverlib/uart.h"      
#include "utils/uartstdio.c"     
#include "driverlib/timer.h"     

// %%%%%%%%%%%%%%%%%%%%%%%% PARA LOS ERRORES %%%%%%%%%%%%%%%%%%%%%%%%
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);  // Bucle infinito para detener el programa en caso de error
}
#endif

void timer0A_handler(void);  // Prototipo de la función de interrupción del Timer0
void timer1A_handler(void);  // Prototipo de la función de interrupción del Timer1

uint32_t FS = 120000000;     // Frecuencia de sistema inicial
uint8_t counter0 = 0;        // Contador utilizado en la interrupción del Timer0

char motor_dc[15];           // Cadena de caracteres para recibir datos del motor DC
uint32_t dc_int;             // Valor entero de la velocidad del motor DC
uint32_t prev_FS;            // Frecuencia de sistema anterior

int main(void)
{      
    // Configurar el sistema a una frecuencia de 120 MHz
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),120000000);
        
    // Enable GPIO port 0,A and F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    // %%%%%%%%%%%%%%%%%%%%%%%% CONFIGURACION UART %%%%%%%%%%%%%%%%%%%%%%%%
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, 0x03);
    
    // %%%%%%%%%%%%%%%%%%%%%%%% CONFIGURACION DEL TIMER 0A %%%%%%%%%%%%%%%%%%%%%%%%
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Configurar el temporizador
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, FS);
    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
    
    // %%%%%%%%%%%%%%%%%%%%%%%% CONFIGURACION DEL TIMER 1A %%%%%%%%%%%%%%%%%%%%%%%%
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // Configurar el temporizador
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, 12000000);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
    
    UARTStdioConfig(0,9600,120000000);  // Configurar UART para entrada/salida estándar
    
    // Verifica si el periférico está habilitado
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 0x02);  // Configurar el pin F1 como salida
    
    while(1)
    {
        // Bucle infinito para mantener el programa en ejecución
    }
}

// Manejador de interrupción del Timer0
void timer0A_handler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A);  // Limpiar la bandera de interrupción
    switch(++counter0) {
        case 1: 
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);  // Encender el LED conectado al pin F1
            break;
        case 2: 
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);  // Apagar el LED conectado al pin F1
            counter0= 0;  // Reiniciar el contador
            break;
    }       
}

// Manejador de interrupción del Timer1
void timer1A_handler(void){
    prev_FS = FS;  // Guardar la frecuencia de sistema anterior
    
    UARTgets(motor_dc,15);  // Leer datos del motor DC desde UART
    dc_int = atoi(motor_dc);  // Convertir la cadena de caracteres a un valor entero
    
    if (dc_int > 50)  // Si la velocidad del motor es mayor que 50
    {
        FS = 120000000;  // Establecer la frecuencia de sistema como 120 MHz
    }
    else if (dc_int <= 50)  // Si la velocidad del motor es menor o igual a 50
    {
        FS = 120000000 * 2;  // Establecer la frecuencia de sistema como 240 MHz
    }        
        
    if (FS != prev_FS)  // Si la frecuencia de sistema cambió
    {
        TimerLoadSet(TIMER0_BASE, TIMER_A, FS);  // Actualizar el valor de carga del Timer0
    }
        
    TimerIntClear(TIMER1_BASE, TIMER_A);  // Limpiar la bandera de interrupción del Timer1
}

