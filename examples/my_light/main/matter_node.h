#pragma once

#include <matter_endpoint.h>

void create_matter_node(endpoint_descriptor_t* endpoints, int endpoints_length);
void start_matter_node();
