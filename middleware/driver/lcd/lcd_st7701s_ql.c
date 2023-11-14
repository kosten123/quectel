/**
 * @file lcd_st7701s_ck.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <driver/gpio.h>
#include <driver/media_types.h>
#include <driver/lcd_types.h>
#include <driver/lcd_spi.h>
#include "bk_misc.h"
#include "lcd_devices.h"
#include "gpio_driver.h"
#include <driver/lcd.h>

#include "../../../components/media/include/media_app.h"

#if CONFIG_PWM
#include <driver/pwm.h>
#endif

#define Delay	delay_ms
#define SPI_WriteComm 	lcd_spi_write_cmd
#define SPI_WriteData 	lcd_spi_write_data

static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_26M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,

	.hsync_pulse_width = 2,
	.vsync_pulse_width = 2,
	.hsync_back_porch = 10, //40,  //3
	.hsync_front_porch = 10, //5,  //2
	.vsync_back_porch = 10, //8,   //1
	.vsync_front_porch = 10, //8,  //1
};

static void lcd_st7701s_config(void)
{
	bk_gpio_set_output_high(LCD_SPI_RST);
	Delay(20);
	bk_gpio_set_output_low(LCD_SPI_RST);
	Delay(100);
	bk_gpio_set_output_high(LCD_SPI_RST);
	Delay(120);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);

	SPI_WriteComm(0xEF);
	SPI_WriteData(0x08);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xC0); //set line number = 854
	SPI_WriteData(0xe9);
	SPI_WriteData(0x03);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x08);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x01);
	SPI_WriteData(0x06);

	SPI_WriteComm(0xC7);
	SPI_WriteData(0x00);//04

	SPI_WriteComm(0xCC);
	SPI_WriteData(0x18);

	SPI_WriteComm(0xB0);
	SPI_WriteData(0x40);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x58);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x11);
	SPI_WriteData(0x06);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x08);
	SPI_WriteData(0x09);
	SPI_WriteData(0x26);
	SPI_WriteData(0x06);
	SPI_WriteData(0x15);
	SPI_WriteData(0x18);
	SPI_WriteData(0x6A);
	SPI_WriteData(0x6E);
	SPI_WriteData(0x4F);

	SPI_WriteComm(0xB1);
	SPI_WriteData(0x40);
	SPI_WriteData(0x11);
	SPI_WriteData(0x57);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x11);
	SPI_WriteData(0x07);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x09);
	SPI_WriteData(0x08);
	SPI_WriteData(0x26);
	SPI_WriteData(0x05);
	SPI_WriteData(0xD3);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x6B);
	SPI_WriteData(0x6E);
	SPI_WriteData(0x4F);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);

	SPI_WriteComm(0xB0);
	SPI_WriteData(0x56);

	SPI_WriteComm(0xB1);
	SPI_WriteData(0x39);

	SPI_WriteComm(0xB2);
	SPI_WriteData(0x87);

	SPI_WriteComm(0xB3);
	SPI_WriteData(0x80);

	SPI_WriteComm(0xB5);
	SPI_WriteData(0x4E);

	SPI_WriteComm(0xB7);
	SPI_WriteData(0x85);

	SPI_WriteComm(0xB8);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xB9);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xBC);
	SPI_WriteData(0x03);

	SPI_WriteComm(0xC0);
	SPI_WriteData(0x89);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x78);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x78);

	SPI_WriteComm(0xD0);
	SPI_WriteData(0x88);

	SPI_WriteComm(0xE0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x02);

	SPI_WriteComm(0xE1);
	SPI_WriteData(0x04);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x20);

	SPI_WriteComm(0xE2);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE3);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE4);
	SPI_WriteData(0x22);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE5);
	SPI_WriteData(0x04);
	SPI_WriteData(0x5C);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x06);
	SPI_WriteData(0x5C);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE6);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE7);
	SPI_WriteData(0x22);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x05);
	SPI_WriteData(0x5C);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x07);
	SPI_WriteData(0x5C);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xEB);
	SPI_WriteData(0x02);
	SPI_WriteData(0x00);
	SPI_WriteData(0x40);
	SPI_WriteData(0x40);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xEC);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xED);
	SPI_WriteData(0xAA);
	SPI_WriteData(0x45);
	SPI_WriteData(0x0B);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xB0);
	SPI_WriteData(0x54);
	SPI_WriteData(0xAA);

	SPI_WriteComm(0xEF);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x54);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0E);

	SPI_WriteComm(0x11);
	Delay(120);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0C);
	Delay(10);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE6);
	SPI_WriteData(0x16);
	SPI_WriteData(0x7C);

	SPI_WriteComm(0x3A);
	SPI_WriteData(0x07); //RGB565

	SPI_WriteComm(0x13);

	SPI_WriteComm(0x36);
	SPI_WriteData(0x18);//08,10


	SPI_WriteComm(0x29);
	Delay(20);


}

static void lcd_st7701s_init(void)
{
#if 1
    os_printf("lcd_st7701s_ql: init.\r\n");
#endif
	lcd_spi_init_gpio();
	lcd_st7701s_config();
	//media_app_lcd_rotate(ROTATE_270);
	bk_lcd_set_partical_display(1,187,667,0,480);
}

static void st7701s_lcd_backlight_open(void)
{
	os_printf("lcd_st7701s_ql: backlight open.\r\n");
#if CONFIG_PWM
	lcd_driver_backlight_init(LCD_RGB_PWM_BACKLIGHT, 100);
#endif
}

static void lcd_set_backlight(uint8_t percent)
{
#if CONFIG_PWM
	pwm_period_duty_config_t config = {0};

	if (percent > 100)
	{
		percent  = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;

	bk_pwm_set_period_duty(LCD_RGB_PWM_BACKLIGHT, &config);
#endif
}

static void st7701s_lcd_backlight_close(void)
{
	os_printf("lcd_st7701s: backlight close.\r\n");
#if CONFIG_PWM
	lcd_driver_backlight_deinit(LCD_RGB_PWM_BACKLIGHT);
#endif
}

const lcd_device_t lcd_device_st7701s_ql =
{
	.id = LCD_DEVICE_ST7701S_QL,
	.name = "st7701s_ql",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X854,//PPI_854X480
	.rgb = &lcd_rgb,
	.backlight_open = st7701s_lcd_backlight_open,
	.backlight_set = lcd_set_backlight,
	.init = lcd_st7701s_init,
	.backlight_close = st7701s_lcd_backlight_close,
	.lcd_off = NULL,
};
