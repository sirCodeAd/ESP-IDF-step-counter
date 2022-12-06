#include "Pins.h"

/*
Initiliaze button and led pins
*/
void Init_Pins(){

    gpio_reset_pin(BUTTON_PIN);
    gpio_reset_pin(LED_PIN);

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);

}

/*
Function that blink LED n-times with delay of n-millisec.
*/
void blink_LEDs(int ms, int times){

    for(int i = 0; i < times; i++){
        
        gpio_set_level(LED_PIN, 1);
        

        vTaskDelay(pdMS_TO_TICKS(ms));

        gpio_set_level(LED_PIN, 0);

        vTaskDelay(pdMS_TO_TICKS(ms));

    }
}