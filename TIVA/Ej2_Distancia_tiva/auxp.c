///                                         LABORATORIO 6
// ULTRASONICO 
// Detect the distance of an object using the HC-SR04 ultrasonic
//sensor in Raspberry Pi. Then send the value of the distance to the
// Tiva with serial communication using UART. Finally follow the next
//instructions:
//                • If the value of distance is above 10 cm, turn of all the user LEDs in the TIVA.
//                • If the value of distance is between 10 – 8 cm, turn on the userLED PN1.
//                • If the value of distance is between 8 – 6 cm, turn on the userLEDs PN1 and PN0.
//                • If the value of distance is between 6 – 4 cm, turn on the userLEDs PN1, PN0 and PF4.
//                • If the value of distance is below 4 cm, turn on all the userLEDs in the TIVA.

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.c"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

void Delay(uint32_t);

char data[100];


int main(void)
{
    // Configuración del reloj del sistema a 120 MHz
    SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);
    
    
    
    // Habilitación del periférico GPIO para los LEDs en el puerto N
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    
    // Verificar si el periférico está habilitado
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)) {
    }
    
    // Configuración de los pines PN0 y PN1 como salida digital
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0); // PN0 como salida
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1); // PN1 como salida
    
      
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        // Habilitar los puertos GPIO N, F y J y A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    //habibilar puerto E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
        // Verifica si los periféricos están habilitados
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)){}
    
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
    
        // Habilitar los GPIO N y F para los LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    
    /*----------GPIO DECLARATION----------*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); // PF2 para LED externo
    
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, 0x03); // PE0 y PE1 como entradas
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    
    GPIOPadConfigSet(GPIO_PORTE_BASE, 0x03, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // Habilitar pull-up
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    
    //PINES ESPECIFICOS
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);

    /*----------UART DECLARATION----------*/  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, 0x03);
    UARTStdioConfig(0, 9600, 120000000);
    
    while(1)
    {
        /*----------UART IMPLEMENTATION----------*/
		if(GPIOPinRead(GPIO_PORTE_BASE, 0x01) == 0)
		{
			while((GPIOPinRead(GPIO_PORTE_BASE, 0x01) == 0))
			{
			}
			UARTprintf("BUTTON 1 PRESSED\n");
			UARTprintf("w\n");
		}

		if(GPIOPinRead(GPIO_PORTE_BASE, 0x02) == 0)
		{
			while((GPIOPinRead(GPIO_PORTE_BASE, 0x02) == 0))
			{
			}
			UARTprintf("BUTTON 2 PRESSED\n");
			UARTprintf("s\n");
		}
		
		if(GPIOPinRead(GPIO_PORTE_BASE, 0x04) == 0)
		{
			while((GPIOPinRead(GPIO_PORTE_BASE, 0x04) == 0))
			{
			}
			UARTprintf("BUTTON 3 PRESSED\n");
			UARTprintf("a\n");
		}
		
		if(GPIOPinRead(GPIO_PORTE_BASE, 0x08) == 0)
		{
			while((GPIOPinRead(GPIO_PORTE_BASE, 0x08) == 0))
			{
			}
			UARTprintf("BUTTON 4 PRESSED\n");
			UARTprintf("d\n");
		}
		
		
		
		
		

		if (UARTCharsAvail(UART0_BASE)){
		
		UARTgets(data, 100);
		
		// Convertir la cadena a un número entero
		int valor = atoi(data);
		
		// Comparar si el valor es mayor a 20
		
		if (valor > 20){
		
		UARTprintf("Distancia mayor a 20 CM\n"); // 
		
		// Encender el LED 
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
		SysCtlDelay((120000000 / 3)*0.1);  // Delay de 1 segundo 
		// Encender el LED en PN1 (sin apagar PN0);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
		// Esperar 1 segund
		SysCtlDelay((120000000 / 3)*0.1);  // Delay de 1 segundo
		// Apagar ambos LEDs
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
		// Esperar 1 segundo antes de repetir
		SysCtlDelay((120000000 / 3)*0.1);  // Delay de 1 segundo
		
		
		
		}
		if ((valor < 20) && (valor > 15)){
		
		UARTprintf("Distancia entre 20 y 15 CM\n"); // 
		
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1); 
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0x0);
		
		}
		if (valor < 10){
		
		UARTprintf("Distancia menor a 10 CM\n"); // 
		
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1); 
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0); 
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4); 
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0); 
		}
	
		Delay(100);
    }
}
}

void Delay(uint32_t time)
{
    SysCtlDelay(40000 * time);
}

