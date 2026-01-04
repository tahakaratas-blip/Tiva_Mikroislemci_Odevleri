#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>

// --- PIN TANIMLAMALARI ---
// Kontrol Portu (RS ve EN Pinleri için)
#define LCD_PORT_CTRL  GPIO_PORTA_BASE
#define RS             GPIO_PIN_2
#define EN             GPIO_PIN_3

// Veri Portu (D4, D5, D6, D7 Pinleri için)
#define LCD_PORT_DATA  GPIO_PORTB_BASE
#define DATA_PINS      (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

// --- FONKSÝYON PROTOTÝPLERÝ ---
void LCD_Init(void);
void LCD_Komut(unsigned char komut);
void LCD_Veri(unsigned char veri);
void LCD_Yaz(char *str);
void LCD_Git(int satir, int sutun);
void LCD_Temizle(void);

#endif /* LCD_H_ */
