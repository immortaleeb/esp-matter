#pragma once

#include <app_driver_handle.h>

#include <esp_matter.h>
#include <esp_err.h>

using namespace esp_matter;

typedef void *endpoint_handle_t;

typedef struct {
    endpoint_handle_t handle;
    uint16_t endpoint_id;
    uint16_t (*register_endpoint)(endpoint_handle_t, node_t*);
    void (*on_start)(endpoint_handle_t);
    esp_err_t (*handle_attribute_update)(
        app_driver_handle_t driver_handle,
        uint16_t endpoint_id,
        uint32_t cluster_id,
        uint32_t attribute_id,
        esp_matter_attr_val_t *val
    );
} endpoint_descriptor_t;
