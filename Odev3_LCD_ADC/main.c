#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"

/* ------------------ Yazýlýmsal Saat ------------------ */
uint8_t hour = 12;
uint8_t min  = 0;
uint8_t sec  = 0;

void UpdateTime(void)
{
    sec++;
    if(sec == 60) { sec = 0; min++; }
    if(min == 60) { min = 0; hour++; }
    if(hour == 24){ hour = 0; }
}

/* ------------------ UART Gönder ------------------ */
void UARTSend(const char *str)
{
    while(*str)
    {
        UARTCharPut(UART0_BASE, *str++);
    }
}

/* ------------------ MAIN ------------------ */
int main(void)
{
    uint32_t adcValue;
    uint8_t  button;
    char buffer[50];

    /* Sistem saat ayarý */
    SysCtlClockSet(SYSCTL_SYSDIV_4 |
                   SYSCTL_USE_PLL |
                   SYSCTL_XTAL_16MHZ |
                   SYSCTL_OSC_MAIN);

    /* UART0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE,
                    GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE,
                        SysCtlClockGet(),
                        9600,
                        (UART_CONFIG_WLEN_8 |
                         UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    /* GPIOF – PF4 Buton */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,
                     GPIO_PIN_4,
                     GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);

    /* ADC0 – AIN0 (PE3) */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3,
                          ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
        ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    /* Ana döngü */
    while(1)
    {
        /* ADC oku */
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false));
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);

        /* PF4 (aktif düþük) */
        button = (GPIOPinRead(GPIO_PORTF_BASE,
                              GPIO_PIN_4) == 0) ? 1 : 0;

        /* Veri paketi */
        sprintf(buffer,
                "%02d:%02d:%02d,%d,%d\n",
                hour, min, sec,
                adcValue, button);

        /* UART gönder */
        UARTSend(buffer);

        /* Saat artýr */
        UpdateTime();

        /* 1 saniye gecikme */
        SysCtlDelay(SysCtlClockGet() / 3);
    }
}
