#ifndef LCD_H
#define LCD_H

void LcdInit(void);
void LcdCommand(unsigned char cmnd);
void LcdChar(unsigned char data);
void LcdString(char *str);
void LcdClear(void);

#endif