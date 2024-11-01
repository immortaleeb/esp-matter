#pragma once

#include <app_driver_handle.h>
#include <matter_endpoint.h>

#include <esp_matter.h>

using namespace esp_matter;

typedef struct {
    uint16_t endpoint_id;
} _light_endpoint_t;

typedef _light_endpoint_t *light_endpoint_t;

endpoint_descriptor_t light_endpoint_create_descriptor();

uint16_t register_light_endpoint(endpoint_handle_t handle, node_t *node);

void light_endpoint_on_start(endpoint_handle_t handle);

esp_err_t handle_light_attribute_update(
    app_driver_handle_t driver_handle,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
);
