// LABORATORIO 6
//eJERCICIO POTENCIOMETRO
//Connect a potentiometer to TIVA and make it vary the FS from a
//custom sequence using timer 0.
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// Función de delay
void Delay(uint32_t time)
{
    SysCtlDelay(40000 * time);  // Aproximadamente 1 ms para un reloj de 120 MHz
}

int main(void)
{
    uint32_t ui32Value;  // Variable para almacenar el valor del ADC

    // Configuración del reloj del sistema a 120 MHz
    SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);

    // Habilitación de periféricos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // Puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); // Puerto N
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  // ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK); // Puerto K para el ADC

    // Configuración del pin para el ADC (Canal 19 -> PK3 en la TM4C1294)
    GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_3);

    // Configuración del ADC
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);  // Secuencia 3, disparo por procesador
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH19);  // Paso único, canal 19
    ADCSequenceEnable(ADC0_BASE, 3);  // Habilitar la secuencia
    ADCIntClear(ADC0_BASE, 3);        // Limpiar interrupciones del ADC

    // Configuración de pines GPIO para LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // LEDs en PN0 y PN1
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);

    while (1)
    {
        // Disparar el ADC
        ADCProcessorTrigger(ADC0_BASE, 3);

        // Esperar a que la conversión termine
        while (!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }

        // Limpiar la interrupción del ADC
        ADCIntClear(ADC0_BASE, 3);

        // Leer el valor del ADC
        ADCSequenceDataGet(ADC0_BASE, 3, &ui32Value);

        // Operar con el valor del ADC
        if (ui32Value > 2047)  // Si el valor es mayor a la mitad del rango (0-4095)
        {
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
            //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1);  // Encender LEDs
        }
        else
        {
        
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0x0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);
            //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);  // Apagar LEDs
        }

        Delay(100);  // Retardo de 100 ms
    }
}

