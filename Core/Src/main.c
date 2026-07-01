
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (TRANSMITTER with DIAGNOSTICS)
  ******************************************************************************
  */
/* USER CODE END Header */
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "lcd16x2_i2c.h"
#include "NRF24.h"
#include "NRF24_reg_addresses.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Magic header — first 4 bytes of every packet. */
#define MAGIC_0  0xDE
#define MAGIC_1  0xAD
#define MAGIC_2  0xBE
#define MAGIC_3  0xEF

/* Declare the read register function from your NRF24 library */
extern uint8_t nrf24_r_reg(uint8_t reg, uint8_t size);

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_TIM1_Init();

    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start(&htim1);

    lcd16x2_i2c_init(&hi2c1);
    lcd16x2_i2c_clear();
    lcd16x2_i2c_setCursor(0, 0);
    lcd16x2_i2c_printf("NRF24 TX Init");
    HAL_Delay(500);

    ce_low();
    csn_high();
    HAL_Delay(10);

    nrf24_pwr_dwn();
    HAL_Delay(5);

    /* Configure NRF24 to MATCH RECEIVER EXACTLY */
    nrf24_set_channel(76);
    nrf24_data_rate(_1mbps);
    nrf24_tx_pwr(_0dbm);
    nrf24_set_addr_width(5);
    nrf24_set_crc(en_crc, _2byte);

    nrf24_auto_ack_all(no_auto_ack);
    nrf24_auto_retr_delay(0);
    nrf24_auto_retr_limit(0);

    nrf24_dpl(disable);
    nrf24_en_ack_pld(disable);
    nrf24_en_dyn_ack(disable);

    /* Open TX pipe with the exact same address as the Receiver */
    uint8_t pipeAddr[5] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
    nrf24_open_tx_pipe(pipeAddr);

    nrf24_flush_tx();
    nrf24_pwr_up();
    HAL_Delay(5);

    /* ====================================================================
     * HARDWARE DIAGNOSTIC TEST (TRANSMITTER)
     * Let's read the RF_CH (Channel) register. We just set it to 76.
     * ==================================================================== */
    uint8_t test_val = nrf24_r_reg(RF_CH, 1);

    lcd16x2_i2c_clear();
    lcd16x2_i2c_setCursor(0, 0);

    if (test_val == 76)
    {
        /* SUCCESS! The STM32 and NRF are communicating properly */
        lcd16x2_i2c_printf("SPI TEST: OK!");
        lcd16x2_i2c_setCursor(1, 0);
        lcd16x2_i2c_printf("Value: %d", test_val);

        HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
        HAL_Delay(2000);
        HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
    }
    else
    {
        /* FAILURE! We read the wrong value. SPI wiring is bad or SPI speed/mode is wrong. */
        lcd16x2_i2c_printf("SPI FAIL: %02X", test_val);
        lcd16x2_i2c_setCursor(1, 0);
        lcd16x2_i2c_printf("CHECK WIRING!");

        /* Trap the code here forever and blink the LED rapidly */
        while (1)
        {
            HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
            HAL_Delay(100);
        }
    }
    /* ==================================================================== */

    uint32_t txCount = 0;

    lcd16x2_i2c_clear();
    lcd16x2_i2c_setCursor(0, 0);
    lcd16x2_i2c_printf("TX Ready!");
    HAL_Delay(1000);

    /* USER CODE END 2 */

    while (1)
    {
        uint8_t buf[32];
        memset(buf, 0, sizeof(buf));

        /* 1. Insert magic bytes at the exact front of payload */
        buf[0] = MAGIC_0;
        buf[1] = MAGIC_1;
        buf[2] = MAGIC_2;
        buf[3] = MAGIC_3;

        /* 2. Format the actual message starting at index 4 */
        txCount++;
        sprintf((char *)&buf[4], "Data %lu", txCount);

        /* 3. Transmit exactly 32 bytes */
        nrf24_transmit(buf, 32);

        /* 4. Visual Feedback (LED + LCD) */
        HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);

        lcd16x2_i2c_clear();
        lcd16x2_i2c_setCursor(0, 0);
        lcd16x2_i2c_printf("TX #%lu Sent", txCount);
        lcd16x2_i2c_setCursor(1, 0);
        lcd16x2_i2c_printf("%.16s", (char *)&buf[4]);

        HAL_Delay(100);
        HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);

        /* 5. Wait 1 second before sending the next packet */
        HAL_Delay(900);
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = 8;
    RCC_OscInitStruct.PLL.PLLN            = 180;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    if (HAL_PWREx_EnableOverDrive()       != HAL_OK) { Error_Handler(); }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) { Error_Handler(); }
}

/* USER CODE BEGIN 4 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
/* USER CODE END 4 */
