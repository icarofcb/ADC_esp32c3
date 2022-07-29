#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include <time.h>
#include <sys/time.h>

#include "driver/adc.h"
#include "driver/gpio.h"
#include <driver/uart.h>
#include "esp_adc_cal.h"

#define BAT ADC1_CHANNEL_4 //GPIO 2
//#define BAT ADC1_CHANNEL_4 //GPIO 4
#define NO_OF_SAMPLES 64

static const char *TAG = "ADC DO ICARO TOMA";
static esp_adc_cal_characteristics_t adc1_chars;

void app_main(void)
{
        //=============// GPIO ADC config //=============//
        esp_err_t ret = ESP_OK;
        uint32_t adc_reading = 0;
        uint32_t voltage = 0;

        ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
        ESP_ERROR_CHECK(adc1_config_channel_atten(BAT, ADC_ATTEN_DB_11));

        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    
    while (1) {

        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
            adc_reading += adc1_get_raw(BAT);
        }

        adc_reading /= NO_OF_SAMPLES;

        ESP_LOGI(TAG, "Raw data: %d", adc_reading);

        voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc1_chars);
        ESP_LOGI(TAG, "voltage: %d mV", voltage);

        adc_reading = 0;
        voltage     = 0;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
