#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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


// pin selection
#define BUTTON 26
#define BUTTON_2 7

/*
if button state is 1 fade led on,
if button state is 2 fade led off,
if button state is 0 do nothing
*/
uint32_t button_state = 0;


// this function sets up data structures for the ledc timer and channel
static void led_pwm_setup()
{
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

	ledc_timer_config(&my_timer_config);
        ledc_channel_config(&my_channel_config);
}

// interrupt handler changing button state to 1 so the running loop fades the led on
static void button_trigger()
{
	button_state = 1;
}


// interrupt handler changing button state to 2 so the running loop fades the led off
static void button_2_trigger()
{
        button_state = 2;
}


//start of main program function with running loop
void app_main(void)
{
	//call function to setup led pwm and configure data structures
	led_pwm_setup();

	// SETUP BUTTONS 1 AND 2 AS INTERRUPTS
	gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
	gpio_set_intr_type(BUTTON, GPIO_INTR_NEGEDGE);

	gpio_set_direction(BUTTON_2, GPIO_MODE_INPUT);
        gpio_set_pull_mode(BUTTON_2, GPIO_PULLDOWN_ONLY);
        gpio_set_intr_type(BUTTON_2, GPIO_INTR_POSEDGE);

	//setup isr and its handlers
	gpio_install_isr_service(0);
	gpio_isr_handler_add(BUTTON, button_trigger, NULL);

	gpio_isr_handler_add(BUTTON_2, button_2_trigger, NULL);

	gpio_intr_enable(BUTTON);

        gpio_intr_enable(BUTTON_2);

	// setup ledc hardware fading capability
	ledc_fade_func_install(0);

	while(1)
	{
		//button state is checked by loop to fade led in while loop.
		// attempting to run ledc_fade_start in interrupt handler was causing watchdog timeout
		if(button_state == 1)
		{
			button_state = 0;

			ledc_set_fade_with_time(SPEED_MODE, CHANNEL, DUTY_CYCLE, FADE_TIME_MS);
			ledc_fade_start(SPEED_MODE, CHANNEL, LEDC_FADE_WAIT_DONE);

		} else if(button_state == 2)
		{
			button_state = 0;

			ledc_set_fade_with_time(SPEED_MODE, CHANNEL, 0, FADE_TIME_MS);
                        ledc_fade_start(SPEED_MODE, CHANNEL, LEDC_FADE_WAIT_DONE);
		};

		vTaskDelay(100 / portTICK_PERIOD_MS);
	};
}
