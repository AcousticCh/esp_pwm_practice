## ledc_ practice

### Fading led's on and off with buttons

This program is meant for practice


#### what I learned:
 - leds cannot be faded inside of interrupt handlers. To solve this issue i created an if statement inside of the main() functions while loop. the if statement checks the button_state variable and will fade the led depending on that state in the loop. button state is changed in the interrrupt handlers.


#### components: 
 - esp32-pico-mini-02u
 - 330 ohm resistor
 - blue led
 - "NO" button


#### Pinout:
 - pin 27 is the output to the blue led with the 330 ohm resistor in series
	 - pin 27 fades the led on and off
 - pin 26 is a button input for an interrupt with a pullup resistor
	 - pin 26 calls the interrupt handler that sets button state to 1(fade on)
 - pin 7 is a button input for an interrupt with a pulldown resistor
         - pin 7 calls the interrupt handler that sets button state to 2(fade off)


**NOTE: if button state is 0 the program does nothing while running**
