#include "lcd16x2_i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* LCD Commands */
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

/* Command Bitfields */
#define LCD_DISPLAY_B       0x01
#define LCD_DISPLAY_C       0x02
#define LCD_DISPLAY_D       0x04

static I2C_HandleTypeDef* lcd16x2_i2cHandle;
static uint8_t LCD_I2C_SLAVE_ADDRESS = 0;
#define LCD_I2C_SLAVE_ADDRESS_0  0x4E
#define LCD_I2C_SLAVE_ADDRESS_1  0x7E

static void lcd16x2_i2c_sendCommand(uint8_t command) {
  uint8_t data_u, data_l;
  uint8_t data_t[4];
  data_u = (command & 0xf0);
  data_l = ((command << 4) & 0xf0);
  data_t[0] = data_u | 0x0C;  // en=1, rs=0
  data_t[1] = data_u | 0x08;  // en=0, rs=0
  data_t[2] = data_l | 0x0C;  // en=1, rs=0
  data_t[3] = data_l | 0x08;  // en=0, rs=0
  HAL_I2C_Master_Transmit(lcd16x2_i2cHandle, LCD_I2C_SLAVE_ADDRESS, (uint8_t *) data_t, 4, 100);
}

static void lcd16x2_i2c_sendData(uint8_t data) {
  uint8_t data_u, data_l;
  uint8_t data_t[4];
  data_u = (data & 0xf0);
  data_l = ((data << 4) & 0xf0);
  data_t[0] = data_u | 0x0D;  // en=1, rs=1
  data_t[1] = data_u | 0x09;  // en=0, rs=1
  data_t[2] = data_l | 0x0D;  // en=1, rs=1
  data_t[3] = data_l | 0x09;  // en=0, rs=1
  HAL_I2C_Master_Transmit(lcd16x2_i2cHandle, LCD_I2C_SLAVE_ADDRESS, (uint8_t *) data_t, 4, 100);
}

bool lcd16x2_i2c_init(I2C_HandleTypeDef *pI2cHandle) {
  HAL_Delay(50);
  lcd16x2_i2cHandle = pI2cHandle;
  if(HAL_I2C_IsDeviceReady(lcd16x2_i2cHandle, LCD_I2C_SLAVE_ADDRESS_0, 1, HAL_MAX_DELAY) == HAL_OK) {
    LCD_I2C_SLAVE_ADDRESS = LCD_I2C_SLAVE_ADDRESS_0;
  } else if(HAL_I2C_IsDeviceReady(lcd16x2_i2cHandle, LCD_I2C_SLAVE_ADDRESS_1, 1, HAL_MAX_DELAY) == HAL_OK) {
    LCD_I2C_SLAVE_ADDRESS = LCD_I2C_SLAVE_ADDRESS_1;
  } else {
    return false;
  }
  lcd16x2_i2c_sendCommand(0x33); HAL_Delay(5);
  lcd16x2_i2c_sendCommand(0x32); HAL_Delay(1);
  lcd16x2_i2c_sendCommand(0x28); HAL_Delay(1);
  lcd16x2_i2c_sendCommand(0x01); HAL_Delay(3);
  lcd16x2_i2c_sendCommand(0x06); HAL_Delay(1);
  lcd16x2_i2c_sendCommand(0x0C); HAL_Delay(1);
  lcd16x2_i2c_sendCommand(0x02); HAL_Delay(3);
  return true;
}

void lcd16x2_i2c_setCursor(uint8_t row, uint8_t col) {
  uint8_t maskData;
  maskData = (col & 0x0F);
  if(row == 0) { maskData |= 0x80; lcd16x2_i2c_sendCommand(maskData); }
  else { maskData |= 0xc0; lcd16x2_i2c_sendCommand(maskData); }
}

void lcd16x2_i2c_1stLine(void) { lcd16x2_i2c_setCursor(0,0); }
void lcd16x2_i2c_2ndLine(void) { lcd16x2_i2c_setCursor(1,0); }
void lcd16x2_i2c_TwoLines(void) { lcd16x2_i2c_sendCommand(LCD_FUNCTIONSET | 0x08); }
void lcd16x2_i2c_OneLine(void) { lcd16x2_i2c_sendCommand(LCD_FUNCTIONSET); }
void lcd16x2_i2c_cursorShow(bool state) {
  if(state) lcd16x2_i2c_sendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_B | LCD_DISPLAY_C | LCD_DISPLAY_D);
  else lcd16x2_i2c_sendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_D);
}
void lcd16x2_i2c_clear(void) { lcd16x2_i2c_sendCommand(LCD_CLEARDISPLAY); HAL_Delay(3); }
void lcd16x2_i2c_display(bool state) {
  if(state) lcd16x2_i2c_sendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_B | LCD_DISPLAY_C | LCD_DISPLAY_D);
  else lcd16x2_i2c_sendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_B | LCD_DISPLAY_C);
}
void lcd16x2_i2c_shiftRight(uint8_t offset) { for(uint8_t i=0; i<offset;i++) lcd16x2_i2c_sendCommand(0x1c); }
void lcd16x2_i2c_shiftLeft(uint8_t offset) { for(uint8_t i=0; i<offset;i++) lcd16x2_i2c_sendCommand(0x18); }
void lcd16x2_i2c_printf(const char* str, ...) {
  char stringArray[20];
  va_list args;
  va_start(args, str);
  vsprintf(stringArray, str, args);
  va_end(args);
  for(uint8_t i=0; i<strlen(stringArray) && i<16; i++) {
    lcd16x2_i2c_sendData((uint8_t)stringArray[i]);
  }
}
