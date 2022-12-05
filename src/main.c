#include <esp_task_wdt.h>
#include <esp_pm.h>
#include <math.h>

#include <freertos/task.h>
#include <freertos/FreeRTOS.h>

#include "buffer.h"
#include "I2C_bus.h"
#include "Pins.h"

#define SDA_PIN         33
#define SCL_PIN         32
#define BUFFER_SIZE     10      //Used this size because i wanted to tracker to update more frequently.
#define SAMPLING_PERIOD 100     //I timed 1 step and it took around 1 sec when walking. so 100 steps is 100 sec
#define ALGO_PERIOD     1000    //I wanted to wait a bit before handling all samples.
#define STEP_GOAL       15

void ISR_Button_Handler();

static void sampling_task(void *arg);
static void algo_task(void *arg);
static void led_task(void *arg);

uint32_t buffer_magnitude;
uint32_t *buffer_data;
static uint32_t step_counter = 0;
struct circular_buffer buffer;

SemaphoreHandle_t xSemaphore = NULL;

void app_main() {

    esp_pm_config_esp32_t config = {

        .min_freq_mhz         = 80,
        .max_freq_mhz         = 240,
        .light_sleep_enable   = true

    };

    esp_pm_configure(&config);

    init_I2C(SDA_PIN, SCL_PIN);
    Init_Pins();

    write_I2C(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);
    write_I2C(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 250);

    xSemaphore = xSemaphoreCreateBinary(); 
    
    buffer_data = (uint32_t *)malloc(BUFFER_SIZE * sizeof(uint32_t));
    init_buffer(&buffer, buffer_data, BUFFER_SIZE);

    xTaskCreate(sampling_task, "sampling_task", 2048, NULL, 1, NULL);
    xTaskCreate(algo_task, "algo_task", 2048, NULL, 1, NULL);
    xTaskCreate(led_task,"led_task", 2048, NULL, 5, NULL);

}

static void sampling_task(void *arg) {

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1){

        int16_t raw_data = 0;
        uint8_t register_buffer   = 0;

        uint32_t buffer_magnitude = read_Axis(register_buffer, raw_data);

        add_element(&buffer, buffer_magnitude);

        //printf("Magnitude: %d\n", (int)buffer_magnitude);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SAMPLING_PERIOD));

    }
}

static void algo_task(void *arg){

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1){

        uint32_t elements = get_elements(&buffer);

        if(elements > 0){

            float mean_value = 0;
            float sd_value   = 0;
            float sum        = 0;
            int n            = 0;
        
            //mean value calculation
            for(int i = 0; i < buffer.max_length; i++){
            
                if(buffer.data[i] != UINT32_MAX){

                mean_value = mean_value + (float)buffer.data[i];
                n++;

                }
            }

            mean_value = mean_value / n;

            //SD value calculation
            for(int i = 0; i < buffer.max_length; i++){

                if(buffer.data[i] != UINT32_MAX){

                    sum = sum + powf((buffer.data[i] - mean_value), 2);

                }
            }

            sum = sum / n;
            sd_value = sqrtf(sum);

            //check if we are moving or standing still
            if(sd_value > 500){

                for (int i = 0; i < buffer.max_length; i++){

                    if(buffer.data[i] != UINT32_MAX){

                        //check if sample is > mean + (SD * k)
                        if((float)remove_head(&buffer) > mean_value + (sd_value * 1.3)){

                        step_counter++;

                        }
                    }
                }
            }
        }

        printf("Steps: %d\n", step_counter);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ALGO_PERIOD));
    }
}

static void led_task(void *arg){

    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);

    gpio_isr_handler_add(BUTTON_PIN, ISR_Button_Handler, NULL);

    while(1){
        
        //Blink if we reached our step goal
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE){
            if(step_counter >= STEP_GOAL){

                blink_LEDs(300, 3);

            }

        }
    }
}

void ISR_Button_Handler(){

    xSemaphoreGiveFromISR(xSemaphore, NULL); 
}