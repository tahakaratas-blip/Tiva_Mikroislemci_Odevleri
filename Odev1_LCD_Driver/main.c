#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

// --- LCD AYARLARI (PORT B) ---
#define LCD_PORT_BASE GPIO_PORTB_BASE
#define RS GPIO_PIN_0
#define E  GPIO_PIN_1

// --- GLOBAL DEÐÝÞKENLER ---
volatile uint32_t saat = 0, dakika = 0, saniye = 0;
char rxBuffer[32];
volatile int rxIndex = 0;
volatile bool yeniVeriGeldi = false;
char lcdMesaj[17] = "---"; // DÜZELTÝLDÝ: 16 karakterlik ekran için buffer büyütüldü

// --- LCD FONKSÝYONLARI (Aynen Kalabilir) ---
void Lcd_Pulse(void) {
    GPIOPinWrite(LCD_PORT_BASE, E, E);
    SysCtlDelay(800);
    GPIOPinWrite(LCD_PORT_BASE, E, 0);
    SysCtlDelay(800);
}

void Lcd_Byte(unsigned char c, int tur) {
    GPIOPinWrite(LCD_PORT_BASE, RS, tur ? RS : 0);
    GPIOPinWrite(LCD_PORT_BASE, 0xF0, (c & 0xF0));
    Lcd_Pulse();
    GPIOPinWrite(LCD_PORT_BASE, 0xF0, (c & 0x0F) << 4);
    Lcd_Pulse();
    SysCtlDelay(4000);
}

void Lcd_Komut(unsigned char komut) { Lcd_Byte(komut, 0); }
void Lcd_Yaz(char *s) { while(*s) Lcd_Byte(*s++, 1); }

void Lcd_Git(int satir, int sutun) {
    if(satir == 1) Lcd_Komut(0x80 + (sutun-1));
    else Lcd_Komut(0xC0 + (sutun-1));
}

void Lcd_Init(void) {
    SysCtlDelay(SysCtlClockGet() / 10);
    GPIOPinWrite(LCD_PORT_BASE, RS, 0);
    GPIOPinWrite(LCD_PORT_BASE, 0xF0, 0x30); Lcd_Pulse(); SysCtlDelay(10000);
    Lcd_Pulse(); SysCtlDelay(10000);
    Lcd_Pulse();
    GPIOPinWrite(LCD_PORT_BASE, 0xF0, 0x20); Lcd_Pulse();
    Lcd_Komut(0x28);
    Lcd_Komut(0x0C);
    Lcd_Komut(0x06);
    Lcd_Komut(0x01);
    SysCtlDelay(SysCtlClockGet() / 100);
}

// --- TIMER KESMESÝ ---
void Timer0IntHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    saniye++;
    if(saniye >= 60) {
        saniye = 0; dakika++;
        if(dakika >= 60) {
            dakika = 0; saat++;
            if(saat >= 24) saat = 0;
        }
    }
}

// --- UART KESMESÝ (VERÝ ALMA) ---
void UARTIntHandler(void) {
    uint32_t status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, status);

    while(UARTCharsAvail(UART0_BASE)) {
        char c = UARTCharGetNonBlocking(UART0_BASE);

        // DÜZELTÝLDÝ: C# kodu bitiþ karakteri olarak '#' gönderiyor.
        if(c == '#') {
            rxBuffer[rxIndex] = '\0';
            rxIndex = 0;
            yeniVeriGeldi = true;
        } else {
            if(rxIndex < 30) rxBuffer[rxIndex++] = c;
        }
    }
}

// --- ANA FONKSÝYON ---
int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    GPIOPinTypeGPIOOutput(LCD_PORT_BASE, 0xFF);
    Lcd_Init();
    Lcd_Git(1,1); Lcd_Yaz("SAAT: 00:00:00");
    Lcd_Git(2,1); Lcd_Yaz("MSJ: BEKLENIYOR");

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // DÜZELTÝLDÝ: C# kodu 9600 Baudrate kullanýyor. Burasý da 9600 olmalý.
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    IntRegister(INT_UART0, UARTIntHandler);
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    IntRegister(INT_TIMER0A, Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);

    IntMasterEnable();

    char zamanStr[16];

    while(1) {
        if(yeniVeriGeldi) {
            // DÜZELTÝLDÝ: C# kodu Saat için 'S' harfi gönderiyor.
            if(rxBuffer[0] == 'S') {
                int h,m,s;
                if(sscanf(&rxBuffer[1], "%d:%d:%d", &h, &m, &s) == 3) {
                    TimerDisable(TIMER0_BASE, TIMER_A);
                    saat=h; dakika=m; saniye=s;
                    TimerEnable(TIMER0_BASE, TIMER_A);
                }
            }
            // DÜZELTÝLDÝ: Metin iþleme geliþtirildi.
            else if(rxBuffer[0] == 'M') {
                // Gelen "MMerhaba" verisinden baþtaki 'M'yi atlayýp kopyalýyoruz
                strncpy(lcdMesaj, &rxBuffer[1], 16);
                lcdMesaj[16] = '\0'; // Garanti olsun diye sonlandýr

                // Ekraný temizlemeden sadece alt satýrý temizle
                Lcd_Git(2, 1);
                Lcd_Yaz("                "); // Satýrý sil
                Lcd_Git(2, 1);
                Lcd_Yaz(lcdMesaj);
            }
            yeniVeriGeldi = false;
        }

        sprintf(zamanStr, "%02d:%02d:%02d", saat, dakika, saniye);
        Lcd_Git(1, 7);
        Lcd_Yaz(zamanStr);

        SysCtlDelay(SysCtlClockGet() / 20);
    }
}
