#pragma once

#include <esp_err.h>
#include "bsp/esp-bsp.h"

#define LIGHT_ON true
#define LIGHT_OFF false

esp_err_t light_set_power(led_indicator_handle_t handle, bool on);

esp_err_t light_set_brightness(led_indicator_handle_t handle, uint8_t brightness);

esp_err_t light_set_hue(led_indicator_handle_t handle, uint8_t hue);

esp_err_t light_set_saturation(led_indicator_handle_t handle, uint8_t saturation);

esp_err_t light_set_temperature(led_indicator_handle_t handle, uint32_t temperature);
