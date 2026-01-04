#include "lcd.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// Alt Fonksiyon: LCD'ye 4-bit veri gönderme mantığı
void LCD_Gonder(unsigned char veri, unsigned char kontrol) {
    // RS: 0 -> Komut, 1 -> Veri
    if(kontrol) GPIOPinWrite(LCD_PORT_CTRL, RS, RS);
    else        GPIOPinWrite(LCD_PORT_CTRL, RS, 0);

    // Üst 4 bit (High Nibble)
    GPIOPinWrite(LCD_PORT_DATA, DATA_PINS, (veri & 0xF0));
    GPIOPinWrite(LCD_PORT_CTRL, EN, EN);
    SysCtlDelay(1000);
    GPIOPinWrite(LCD_PORT_CTRL, EN, 0);

    // Alt 4 bit (Low Nibble)
    GPIOPinWrite(LCD_PORT_DATA, DATA_PINS, (veri << 4));
    GPIOPinWrite(LCD_PORT_CTRL, EN, EN);
    SysCtlDelay(1000);
    GPIOPinWrite(LCD_PORT_CTRL, EN, 0);

    SysCtlDelay(20000); // İşlem süresi beklemesi
}

void LCD_Komut(unsigned char komut) { LCD_Gonder(komut, 0); }
void LCD_Veri(unsigned char veri)   { LCD_Gonder(veri, 1); }

void LCD_Init(void) {
    // Portları Aktif Et
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Çıkış olarak ayarla
    GPIOPinTypeGPIOOutput(LCD_PORT_CTRL, RS | EN);
    GPIOPinTypeGPIOOutput(LCD_PORT_DATA, DATA_PINS);

    SysCtlDelay(50000); // Başlangıç beklemesi

    LCD_Komut(0x28); // 4-bit mod, 2 satır, 5x7 font
    LCD_Komut(0x0C); // Ekran açık, imleç kapalı
    LCD_Komut(0x06); // Sağa kaydırma modu
    LCD_Komut(0x01); // Temizle
    SysCtlDelay(50000);
}

void LCD_Yaz(char *str) {
    while(*str) LCD_Veri(*str++);
}

void LCD_Git(int satir, int sutun) {
    if(satir == 1) LCD_Komut(0x80 + (sutun - 1));
    else           LCD_Komut(0xC0 + (sutun - 1));
}

void LCD_Temizle(void) {
    LCD_Komut(0x01);
    SysCtlDelay(50000);
}
