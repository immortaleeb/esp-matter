/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <nvs_flash.h>

#include <app_reset.h>
#include <platform/ESP32/OpenthreadLauncher.h>

#include <app_priv.h>
#include <matter_node.h>
#include <light.h>
#include <light_endpoint.h>

extern "C" void app_main()
{
    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    light_endpoint_t light_endpoint = light_endpoint_create();

    /* Initialize driver */
    app_driver_handle_t button_handle = app_driver_button_init(light_endpoint);
    app_reset_button_register(button_handle);

    create_matter_node(light_endpoint);

    /* Set OpenThread platform config */
    esp_openthread_platform_config_t config = {
        .radio_config = ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_OPENTHREAD_DEFAULT_HOST_CONFIG(),
        .port_config = ESP_OPENTHREAD_DEFAULT_PORT_CONFIG(),
    };
    set_openthread_platform_config(&config);

    start_matter_node();
}
