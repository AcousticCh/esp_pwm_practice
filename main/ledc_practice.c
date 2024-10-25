#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"


//timer config variables
#define SPEED_MODE LEDC_LOW_SPEED_MODE
#define DUTY_CYCLE_BITS LEDC_TIMER_13_BIT
#define TIMER_NUM LEDC_TIMER_0
#define FREQ_HZ (4000)
#define CLOCK_CFG LEDC_AUTO_CLK

ledc_timer_config_t my_timer_config = {
		.speed_mode = SPEED_MODE,
		.timer_num = TIMER_NUM,
		.duty_resolution = DUTY_CYCLE_BITS,
		.freq_hz = FREQ_HZ,
		.clk_cfg = CLOCK_CFG

};

//channel config variables
#define CHANNEL LEDC_CHANNEL_0
#define DUTY_CYCLE (4096)

ledc_channel_config_t my_channel_config = {
		.gpio_num = 27,
		.speed_mode = SPEED_MODE,
		.channel = CHANNEL,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = TIMER_NUM,
		.duty = DUTY_CYCLE,
		.hpoint = 0
};

void app_main(void)
{
	ledc_timer_config(&my_timer_config);
	ledc_channel_config(&my_channel_config);
}
