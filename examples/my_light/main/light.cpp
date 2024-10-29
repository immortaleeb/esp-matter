#include <light.h>

esp_err_t light_set_power(led_indicator_handle_t handle, bool on)
{
    return led_indicator_start(handle, on ? BSP_LED_ON : BSP_LED_OFF);
}

esp_err_t light_set_brightness(led_indicator_handle_t handle, uint8_t brightness)
{
//    int value = REMAP_TO_RANGE(brightness, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);
    return led_indicator_set_brightness(handle, brightness);
}

esp_err_t light_set_hue(led_indicator_handle_t handle, uint8_t hue)
{
//    int value = REMAP_TO_RANGE(val->val.u8, MATTER_HUE, STANDARD_HUE);
    led_indicator_ihsv_t hsv;
    hsv.value = led_indicator_get_hsv(handle);
    hsv.h = hue;
    return led_indicator_set_hsv(handle, hsv.value);
}

esp_err_t light_set_saturation(led_indicator_handle_t handle, uint8_t saturation)
{
//    int value = REMAP_TO_RANGE(val->val.u8, MATTER_SATURATION, STANDARD_SATURATION);
    led_indicator_ihsv_t hsv;
    hsv.value = led_indicator_get_hsv(handle);
    hsv.s = saturation;
    return led_indicator_set_hsv(handle, hsv.value);
}

esp_err_t light_set_temperature(led_indicator_handle_t handle, uint32_t temperature)
{
//    uint32_t value = REMAP_TO_RANGE_INVERSE(val->val.u16, STANDARD_TEMPERATURE_FACTOR);
    return led_indicator_set_color_temperature(handle, temperature);
}
