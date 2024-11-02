#if LIGHT_MODE == LIGHT_MODE_SINGLE_LED

#include <light.h>

#include <esp_log.h>
#include "driver/gpio.h"

static const char *TAG = "light";

#define LED_PIN GPIO_NUM_2

light_handle_t app_driver_light_init()
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    return NULL;
}

esp_err_t light_set_power(light_handle_t handle, bool on)
{
    ESP_LOGI(TAG, "setting power to %d", on);
    return gpio_set_level(LED_PIN, on ? 1 : 0);
}

esp_err_t light_set_brightness(light_handle_t handle, uint8_t brightness)
{
    ESP_LOGI(TAG, "setting brightness to %u", brightness);
    return ESP_OK;
}

esp_err_t light_set_hue(light_handle_t handle, uint8_t hue)
{
    ESP_LOGI(TAG, "setting hue to %u", hue);
    return ESP_OK;
}

esp_err_t light_set_saturation(light_handle_t handle, uint8_t saturation)
{
    ESP_LOGI(TAG, "setting saturation to %u", saturation);
    return ESP_OK;
}

esp_err_t light_set_temperature(light_handle_t handle, uint32_t temperature)
{
    ESP_LOGI(TAG, "setting temperature to %lu", temperature);
    return ESP_OK;
}

esp_err_t light_set_rgb(light_handle_t handle, uint8_t r, uint8_t g, uint8_t b) {
    ESP_LOGI(TAG, "setting rgb to (%u, %u, %u)", r, g, b);
    return ESP_OK;
}

#endif