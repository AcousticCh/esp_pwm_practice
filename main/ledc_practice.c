#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"


//timer config variables
#define SPEED_MODE LEDC_LOW_SPEED_MODE
#define DUTY_CYCLE_BITS LEDC_TIMER_13_BIT
#define TIMER_NUM LEDC_TIMER_0
#define FREQ_HZ (4000)
#define CLOCK_CFG LEDC_AUTO_CLK

//channel config variables
#define CHANNEL LEDC_CHANNEL_0
#define DUTY_CYCLE (4096)

//amount of time to complete fade
#define FADE_TIME_MS 5000



ledc_timer_config_t my_timer_config = {
		.speed_mode = SPEED_MODE,
		.timer_num = TIMER_NUM,
		.duty_resolution = DUTY_CYCLE_BITS,
		.freq_hz = FREQ_HZ,
		.clk_cfg = CLOCK_CFG

};


ledc_channel_config_t my_channel_config = {
		.gpio_num = 27,
		.speed_mode = SPEED_MODE,
		.channel = CHANNEL,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = TIMER_NUM,
		.duty = 0,
		.hpoint = 0
};



void app_main(void)
{
	ledc_timer_config(&my_timer_config);
	ledc_channel_config(&my_channel_config);

	ledc_fade_func_install(0);

	ledc_set_fade_with_time(SPEED_MODE, CHANNEL, DUTY_CYCLE, FADE_TIME_MS);
	while(1)
	{
		ledc_fade_start(SPEED_MODE, CHANNEL, LEDC_FADE_WAIT_DONE);
	};
}
