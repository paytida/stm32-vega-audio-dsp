#ifndef LCD16X2_I2C_H_
#define LCD16X2_I2C_H_

#include "main.h"
#include <stdbool.h>

bool lcd16x2_i2c_init(I2C_HandleTypeDef *pI2cHandle);
void lcd16x2_i2c_setCursor(uint8_t row, uint8_t col);
void lcd16x2_i2c_1stLine(void);
void lcd16x2_i2c_2ndLine(void);
void lcd16x2_i2c_TwoLines(void);
void lcd16x2_i2c_OneLine(void);
void lcd16x2_i2c_cursorShow(bool state);
void lcd16x2_i2c_clear(void);
void lcd16x2_i2c_display(bool state);
void lcd16x2_i2c_shiftRight(uint8_t offset);
void lcd16x2_i2c_shiftLeft(uint8_t offset);
void lcd16x2_i2c_printf(const char* str, ...);

#endif /* LCD16X2_I2C_H_ */
