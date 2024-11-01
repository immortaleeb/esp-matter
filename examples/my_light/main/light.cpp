#include <light.h>

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
//    int value = REMAP_TO_RANGE(brightness, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);
    return led_indicator_set_brightness((led_indicator_handle_t) handle, brightness);
}

esp_err_t light_set_hue(light_handle_t handle, uint8_t hue)
{
//    int value = REMAP_TO_RANGE(val->val.u8, MATTER_HUE, STANDARD_HUE);
    led_indicator_ihsv_t hsv;
    hsv.value = led_indicator_get_hsv((led_indicator_handle_t) handle);
    hsv.h = hue;
    return led_indicator_set_hsv((led_indicator_handle_t) handle, hsv.value);
}

esp_err_t light_set_saturation(light_handle_t handle, uint8_t saturation)
{
//    int value = REMAP_TO_RANGE(val->val.u8, MATTER_SATURATION, STANDARD_SATURATION);
    led_indicator_ihsv_t hsv;
    hsv.value = led_indicator_get_hsv((led_indicator_handle_t) handle);
    hsv.s = saturation;
    return led_indicator_set_hsv((led_indicator_handle_t) handle, hsv.value);
}

esp_err_t light_set_temperature(light_handle_t handle, uint32_t temperature)
{
//    uint32_t value = REMAP_TO_RANGE_INVERSE(val->val.u16, STANDARD_TEMPERATURE_FACTOR);
    return led_indicator_set_color_temperature((led_indicator_handle_t) handle, temperature);
}
