#include <light.h>

#include <esp_log.h>

static const char *TAG = "light";

light_handle_t app_driver_light_init()
{
    /* Initialize led */
    led_indicator_handle_t leds[CONFIG_BSP_LEDS_NUM];
    ESP_ERROR_CHECK(bsp_led_indicator_create(leds, NULL, CONFIG_BSP_LEDS_NUM));
    led_indicator_set_hsv(leds[0], SET_HSV(DEFAULT_HUE, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS));

    return (light_handle_t)leds[0];
}

esp_err_t light_set_power(light_handle_t handle, bool on)
{
    return led_indicator_start((led_indicator_handle_t) handle, on ? BSP_LED_ON : BSP_LED_OFF);
}

esp_err_t light_set_brightness(light_handle_t handle, uint8_t brightness)
{
    ESP_LOGI(TAG, "setting brightness to %u", brightness);
    return led_indicator_set_brightness((led_indicator_handle_t) handle, brightness);
}

esp_err_t light_set_hue(light_handle_t handle, uint8_t hue)
{
    ESP_LOGI(TAG, "setting hue to %u", hue);
    led_indicator_ihsv_t hsv;
    hsv.value = led_indicator_get_hsv((led_indicator_handle_t) handle);
    hsv.h = hue;
    return led_indicator_set_hsv((led_indicator_handle_t) handle, hsv.value);
}

esp_err_t light_set_saturation(light_handle_t handle, uint8_t saturation)
{
    ESP_LOGI(TAG, "setting saturation to %u", saturation);
    led_indicator_ihsv_t hsv;
    hsv.value = led_indicator_get_hsv((led_indicator_handle_t) handle);
    hsv.s = saturation;
    return led_indicator_set_hsv((led_indicator_handle_t) handle, hsv.value);
}

esp_err_t light_set_temperature(light_handle_t handle, uint32_t temperature)
{
    ESP_LOGI(TAG, "setting temperature to %lu", temperature);
    return led_indicator_set_color_temperature((led_indicator_handle_t) handle, temperature);
}

esp_err_t light_set_rgb(light_handle_t handle, uint8_t r, uint8_t g, uint8_t b) {
    ESP_LOGI(TAG, "setting rgb to (%u, %u, %u)", r, g, b);
    uint32_t irgb = (0 << 24) + (r << 16) + (g << 8) + (b << 0);
    return led_indicator_set_rgb((led_indicator_handle_t) handle, irgb);
}
