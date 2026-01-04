#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// --- LCD PIN TANIMLAMALARI ---
// Kontrol Pinleri (Port A)
#define LCD_PORT_CTRL  GPIO_PORTA_BASE
#define RS             GPIO_PIN_2
#define EN             GPIO_PIN_3

// Veri Pinleri (Port B)
#define LCD_PORT_DATA  GPIO_PORTB_BASE
#define DATA_PINS      (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

// --- FONKSÝYON PROTOTÝPLERÝ ---
void LCD_Gonder(unsigned char veri, unsigned char kontrol);
void LCD_Komut(unsigned char komut);
void LCD_Veri(unsigned char veri);
void LCD_Init(void);
void LCD_Git(int satir, int sutun);
void LCD_Yaz(char *str);

// --- ANA PROGRAM ---
int main(void) {
    // Saat hýzýný 40 MHz olarak ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // LCD Baþlatma
    LCD_Init();

    // Ekrana Yazý Yazma
    LCD_Git(1, 1);           // 1. Satýr 1. Sütun
    LCD_Yaz("Tiva C LCD");

    LCD_Git(2, 1);           // 2. Satýr 1. Sütun
    LCD_Yaz("Odev 1 Hazir!");

    while(1) {
        // Sonsuz döngü
    }
}

// --- LCD SÜRÜCÜ FONKSÝYONLARI ---

void LCD_Gonder(unsigned char veri, unsigned char kontrol) {
    // RS Ayarý (0: Komut Yazma, 1: Veri Yazma)
    if(kontrol) GPIOPinWrite(LCD_PORT_CTRL, RS, RS);
    else        GPIOPinWrite(LCD_PORT_CTRL, RS, 0);

    // 1. ADIM: Üst 4 bit (High Nibble) gönderiliyor
    GPIOPinWrite(LCD_PORT_DATA, DATA_PINS, (veri & 0xF0));
    GPIOPinWrite(LCD_PORT_CTRL, EN, EN);
    SysCtlDelay(1000);
    GPIOPinWrite(LCD_PORT_CTRL, EN, 0);

    // 2. ADIM: Alt 4 bit (Low Nibble) gönderiliyor
    GPIOPinWrite(LCD_PORT_DATA, DATA_PINS, (veri << 4));
    GPIOPinWrite(LCD_PORT_CTRL, EN, EN);
    SysCtlDelay(1000);
    GPIOPinWrite(LCD_PORT_CTRL, EN, 0);

    SysCtlDelay(20000); // LCD'nin veriyi iþlemesi için kýsa bekleme
}

void LCD_Komut(unsigned char komut) {
    LCD_Gonder(komut, 0);
}

void LCD_Veri(unsigned char veri) {
    LCD_Gonder(veri, 1);
}

void LCD_Init(void) {
    // Portlarý aktif et
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Pinleri çýkýþ olarak ayarla
    GPIOPinTypeGPIOOutput(LCD_PORT_CTRL, RS | EN);
    GPIOPinTypeGPIOOutput(LCD_PORT_DATA, DATA_PINS);

    SysCtlDelay(50000); // LCD'nin kendine gelmesi için açýlýþ beklemesi

    // 4-bit baþlatma komutlarý
    LCD_Komut(0x28); // 4-bit mod, 2 satýr, 5x7 karakter yapýsý
    LCD_Komut(0x0C); // Ekran açýk, imleç (cursor) kapalý
    LCD_Komut(0x06); // Karakter yazýldýkça imleç saða kaysýn
    LCD_Komut(0x01); // Ekraný tamamen temizle
    SysCtlDelay(50000);
}

void LCD_Yaz(char *str) {
    while(*str) {
        LCD_Veri(*str++);
    }
}

void LCD_Git(int satir, int sutun) {
    if(satir == 1)
        LCD_Komut(0x80 + (sutun - 1));
    else
        LCD_Komut(0xC0 + (sutun - 1));
}
