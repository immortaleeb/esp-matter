#pragma once

#include <app_driver_handle.h>

#include <esp_matter.h>

using namespace esp_matter;

void register_light_endpoint(node_t *node);

void light_endpoint_on_start();

esp_err_t handle_light_attribute_update(
    app_driver_handle_t driver_handle,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
);
