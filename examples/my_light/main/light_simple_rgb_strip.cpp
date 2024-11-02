#include <light.h>

#include <esp_log.h>
#include "driver/ledc.h"

#define RED_PIN     4
#define GREEN_PIN   3
#define BLUE_PIN    2

#define MY_LEDC_MODE          LEDC_LOW_SPEED_MODE
#define MY_LEDC_DUTY_RES      LEDC_TIMER_13_BIT  // 13 bit resolution, 0-8191
#define LEDC_FREQUENCY     5000               // 5 kHz

// Channel definitions
#define LEDC_CHANNEL_RED    LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN  LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE   LEDC_CHANNEL_2

static const char *TAG = "light";

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness;
} _rgb_strip_handle_t;

typedef _rgb_strip_handle_t *rgb_strip_handle_t;

static void setup_rgb_strip() {
    // Timer configuration (can be shared between channels)
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = MY_LEDC_MODE,
        .duty_resolution  = MY_LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz         = LEDC_FREQUENCY,
        .clk_cfg         = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Channel configuration for Red
    ledc_channel_config_t red_channel = {
        .gpio_num       = RED_PIN,
        .speed_mode     = MY_LEDC_MODE,
        .channel        = LEDC_CHANNEL_RED,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty          = 0,
        .hpoint        = 0
    };
    ledc_channel_config(&red_channel);

    // Channel configuration for Green
    ledc_channel_config_t green_channel = {
        .gpio_num       = GREEN_PIN,
        .speed_mode     = MY_LEDC_MODE,
        .channel        = LEDC_CHANNEL_GREEN,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty          = 0,
        .hpoint        = 0
    };
    ledc_channel_config(&green_channel);

    // Channel configuration for Blue
    ledc_channel_config_t blue_channel = {
        .gpio_num       = BLUE_PIN,
        .speed_mode     = MY_LEDC_MODE,
        .channel        = LEDC_CHANNEL_BLUE,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty          = 0,
        .hpoint        = 0
    };
    ledc_channel_config(&blue_channel);
}

esp_err_t set_rgb(uint8_t red, uint8_t green, uint8_t blue) {
    // Convert 8-bit colors to 13-bit duty values
    uint32_t red_duty = (red * 8191) / 255;
    uint32_t green_duty = (green * 8191) / 255;
    uint32_t blue_duty = (blue * 8191) / 255;

    esp_err_t err = ESP_OK;

    err |= ledc_set_duty(MY_LEDC_MODE, LEDC_CHANNEL_RED, red_duty);
    err |= ledc_set_duty(MY_LEDC_MODE, LEDC_CHANNEL_GREEN, green_duty);
    err |= ledc_set_duty(MY_LEDC_MODE, LEDC_CHANNEL_BLUE, blue_duty);

    err |= ledc_update_duty(MY_LEDC_MODE, LEDC_CHANNEL_RED);
    err |= ledc_update_duty(MY_LEDC_MODE, LEDC_CHANNEL_GREEN);
    err |= ledc_update_duty(MY_LEDC_MODE, LEDC_CHANNEL_BLUE);

    return err;
}

esp_err_t set_rgb(rgb_strip_handle_t handle) {
    return set_rgb(
        handle->red * handle->brightness / 255,
        handle->green * handle->brightness / 255,
        handle->blue * handle->brightness / 255
    );
}

light_handle_t app_driver_light_init()
{
    setup_rgb_strip();

    rgb_strip_handle_t handle = (rgb_strip_handle_t)malloc(sizeof(_rgb_strip_handle_t));
    handle->red = 127;
    handle->green = 127;
    handle->blue = 127;
    handle->brightness = 127;
    return (light_handle_t)handle;
}

esp_err_t light_set_power(light_handle_t handle, bool on)
{
    ESP_LOGI(TAG, "setting power to %d", on);

    if (on) {
        return set_rgb((rgb_strip_handle_t)handle);
    } else {
        return set_rgb(0, 0, 0);
    }
}

esp_err_t light_set_brightness(light_handle_t handle, uint8_t brightness)
{
    ESP_LOGI(TAG, "setting brightness to %u", brightness);
    rgb_strip_handle_t rgb_handle = (rgb_strip_handle_t)handle;
    rgb_handle->brightness = brightness;
    return set_rgb(rgb_handle);
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
    rgb_strip_handle_t rgb_handle = (rgb_strip_handle_t)handle;
    rgb_handle->red = r;
    rgb_handle->green = g;
    rgb_handle->blue = b;
    return set_rgb(rgb_handle);
}
