#pragma once

#include <app_driver_handle.h>

#include <esp_matter.h>

using namespace esp_matter;

void register_light_endpoint(node_t *node, app_driver_handle_t &light_handle);

void light_endpoint_on_start();
