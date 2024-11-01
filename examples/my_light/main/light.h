#pragma once

#include <app_driver_handle.h>

#include <esp_err.h>
#include "bsp/esp-bsp.h"

#define LIGHT_ON true
#define LIGHT_OFF false

#define DEFAULT_POWER LIGHT_OFF
#define DEFAULT_BRIGHTNESS 64
#define DEFAULT_HUE 128
#define DEFAULT_SATURATION 254

typedef void *light_handle_t;

light_handle_t app_driver_light_init();

esp_err_t light_set_power(light_handle_t handle, bool on);

esp_err_t light_set_brightness(light_handle_t handle, uint8_t brightness);

esp_err_t light_set_hue(light_handle_t handle, uint8_t hue);

esp_err_t light_set_saturation(light_handle_t handle, uint8_t saturation);

esp_err_t light_set_temperature(light_handle_t handle, uint32_t temperature);
