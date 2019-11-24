/*  
    Microchip Atmel ATSAM3x 2019 Georgi Angelov
        http://www.wizio.eu/    
        https://github.com/Wiz-IO/platform-sam-lora

    OPEN: 'platformio.ini' and edit your settings
 */

#include <samr3.h>
#include <port.h>
#include <clock.h>
#include <gclk.h>
#include <system.h>
#include <samr34_xplained_pro.h>

void SysTick_Handler(void)
{
	port_pin_toggle_output_level(LED_0_PIN);
}

static void config_led(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_0_PIN, &pin_conf);
	port_pin_set_output_level(LED_0_PIN, LED_0_INACTIVE);
}

int main(void)
{
	system_init();

	/*Configure system tick to generate periodic interrupts */
	SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0));

	config_led();

	while (true) {
	}
}