#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"

void Delay(uint32_t time);

int main(void)
{
    uint32_t width = 1; // Duty cycle inicial
    
    /*----------CLOCK DECLARATION----------*/
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    /*----------PWM CLOCK ACTIVATION----------*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);  // Activar módulo PWM0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Activar GPIO para PF1

    /*----------GPIO CONFIGURATION FOR PWM----------*/
    GPIOPinConfigure(GPIO_PF1_M0PWM1);          // Configurar PF1 como M0PWM1
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1); // Configurar PF1 como salida PWM

    /*----------PWM CONFIGURATION----------*/
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // Configurar generador
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 1000); // Período del PWM (ajustar según frecuencia deseada)
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);          // Habilitar el generador
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true); // Activar salida de PWM en M0PWM1

    while (1)
    {
        /*----------UPDATE PWM DUTY CYCLE----------*/
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, width); // Establecer duty cycle
        width++; // Incrementar ancho del pulso
        if (width >= 1000) // Reiniciar duty cycle si supera el período
        {
            width = 1;
        }

        Delay(10); // Pequeña espera para observar el cambio gradual
    }
}

void Delay(uint32_t time)
{
    SysCtlDelay(40000 * time); // Función de retardo (ajustar según necesidad)
}

