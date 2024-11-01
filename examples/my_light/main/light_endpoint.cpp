#include <light_endpoint.h>

#include <light.h>

#include <esp_log.h>
#include <esp_matter.h>
#include <common_macros.h>

/** Standard max values (used for remapping attributes) */
#define STANDARD_BRIGHTNESS 255
#define STANDARD_HUE 360
#define STANDARD_SATURATION 255
#define STANDARD_TEMPERATURE_FACTOR 1000000

/** Matter max values (used for remapping attributes) */
#define MATTER_BRIGHTNESS 254
#define MATTER_HUE 254
#define MATTER_SATURATION 254
#define MATTER_TEMPERATURE_FACTOR 1000000

using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

static const char *TAG = "light_endpoint";

static void configure_level_control_cluster(endpoint_t *endpoint) {
    /* Mark deferred persistence for some attributes that might be changed rapidly */
    cluster_t *level_control_cluster = cluster::get(endpoint, LevelControl::Id);
    attribute_t *current_level_attribute = attribute::get(level_control_cluster, LevelControl::Attributes::CurrentLevel::Id);
    attribute::set_deferred_persistence(current_level_attribute);
}

static void configure_color_control_cluster(endpoint_t *endpoint) {
    cluster_t *color_control_cluster = cluster::get(endpoint, ColorControl::Id);
    attribute_t *current_x_attribute = attribute::get(color_control_cluster, ColorControl::Attributes::CurrentX::Id);
    attribute::set_deferred_persistence(current_x_attribute);
    attribute_t *current_y_attribute = attribute::get(color_control_cluster, ColorControl::Attributes::CurrentY::Id);
    attribute::set_deferred_persistence(current_y_attribute);
    attribute_t *color_temp_attribute = attribute::get(color_control_cluster, ColorControl::Attributes::ColorTemperatureMireds::Id);
    attribute::set_deferred_persistence(color_temp_attribute);
}

uint16_t register_light_endpoint(endpoint_handle_t handle, node_t *node) {
    light_endpoint_t light_endpoint = (light_endpoint_t) handle;
    app_driver_handle_t light_handle = app_driver_light_init();

    extended_color_light::config_t light_config;
    light_config.on_off.on_off = DEFAULT_POWER;
    light_config.on_off.lighting.start_up_on_off = nullptr;
    light_config.level_control.current_level = DEFAULT_BRIGHTNESS;
    light_config.level_control.on_level = DEFAULT_BRIGHTNESS;
    light_config.level_control.lighting.start_up_current_level = DEFAULT_BRIGHTNESS;
    light_config.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
    light_config.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
    light_config.color_control.color_temperature.startup_color_temperature_mireds = nullptr;

    // endpoint handles can be used to add/modify clusters.
    endpoint_t *endpoint = extended_color_light::create(node, &light_config, ENDPOINT_FLAG_NONE, light_handle);
    ABORT_APP_ON_FAILURE(endpoint != nullptr, ESP_LOGE(TAG, "Failed to create extended color light endpoint"));

    uint16_t light_endpoint_id = endpoint::get_id(endpoint);
    ESP_LOGI(TAG, "Light created with endpoint_id %d", light_endpoint_id);
    light_endpoint->endpoint_id = light_endpoint_id;

    configure_level_control_cluster(endpoint);
    configure_color_control_cluster(endpoint);

    return light_endpoint_id;
}

/* Do any conversions/remapping for the actual value here */
 static esp_err_t app_driver_light_set_power(led_indicator_handle_t handle, esp_matter_attr_val_t *val)
 {
     return light_set_power(handle, val->val.b ? LIGHT_ON : LIGHT_OFF);
 }

 static esp_err_t app_driver_light_set_brightness(led_indicator_handle_t handle, esp_matter_attr_val_t *val)
 {
     int brightness = REMAP_TO_RANGE(val->val.u8, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);
     return light_set_brightness(handle, brightness);
 }

 static esp_err_t app_driver_light_set_hue(led_indicator_handle_t handle, esp_matter_attr_val_t *val)
 {
     int value = REMAP_TO_RANGE(val->val.u8, MATTER_HUE, STANDARD_HUE);
     return light_set_hue(handle, value);
 }

 static esp_err_t app_driver_light_set_saturation(led_indicator_handle_t handle, esp_matter_attr_val_t *val)
 {
     int value = REMAP_TO_RANGE(val->val.u8, MATTER_SATURATION, STANDARD_SATURATION);
     return light_set_saturation(handle, value);
 }

 static esp_err_t app_driver_light_set_temperature(led_indicator_handle_t handle, esp_matter_attr_val_t *val)
 {
     uint32_t value = REMAP_TO_RANGE_INVERSE(val->val.u16, STANDARD_TEMPERATURE_FACTOR);
     return light_set_temperature(handle, value);
 }

static esp_err_t app_driver_light_set_defaults(uint16_t endpoint_id) {
   esp_err_t err = ESP_OK;
   void *priv_data = endpoint::get_priv_data(endpoint_id);
   led_indicator_handle_t handle = (led_indicator_handle_t)priv_data;
   node_t *node = node::get();
   endpoint_t *endpoint = endpoint::get(node, endpoint_id);
   cluster_t *cluster = NULL;
   attribute_t *attribute = NULL;
   esp_matter_attr_val_t val = esp_matter_invalid(NULL);

   /* Setting brightness */
   cluster = cluster::get(endpoint, LevelControl::Id);
   attribute = attribute::get(cluster, LevelControl::Attributes::CurrentLevel::Id);
   attribute::get_val(attribute, &val);
   err |= app_driver_light_set_brightness(handle, &val);

   /* Setting color */
   cluster = cluster::get(endpoint, ColorControl::Id);
   attribute = attribute::get(cluster, ColorControl::Attributes::ColorMode::Id);
   attribute::get_val(attribute, &val);
   if (val.val.u8 == (uint8_t)ColorControl::ColorMode::kCurrentHueAndCurrentSaturation) {
       /* Setting hue */
       attribute = attribute::get(cluster, ColorControl::Attributes::CurrentHue::Id);
       attribute::get_val(attribute, &val);
       err |= app_driver_light_set_hue(handle, &val);
       /* Setting saturation */
       attribute = attribute::get(cluster, ColorControl::Attributes::CurrentSaturation::Id);
       attribute::get_val(attribute, &val);
       err |= app_driver_light_set_saturation(handle, &val);
   } else if (val.val.u8 == (uint8_t)ColorControl::ColorMode::kColorTemperature) {
       /* Setting temperature */
       attribute = attribute::get(cluster, ColorControl::Attributes::ColorTemperatureMireds::Id);
       attribute::get_val(attribute, &val);
       err |= app_driver_light_set_temperature(handle, &val);
   } else {
       ESP_LOGE(TAG, "Color mode not supported");
   }

   /* Setting power */
   cluster = cluster::get(endpoint, OnOff::Id);
   attribute = attribute::get(cluster, OnOff::Attributes::OnOff::Id);
   attribute::get_val(attribute, &val);
   err |= app_driver_light_set_power(handle, &val);

   return err;
}

void light_endpoint_on_start(endpoint_handle_t handle) {
    light_endpoint_t light_endpoint = (light_endpoint_t) handle;
    app_driver_light_set_defaults(light_endpoint->endpoint_id);
}

static esp_err_t handle_on_off_attribute_update(
    led_indicator_handle_t &handle,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
) {
    if (attribute_id == OnOff::Attributes::OnOff::Id) {
        return app_driver_light_set_power(handle, val);
    }

    return ESP_OK;
}

static esp_err_t handle_level_control_attribute_update(
    led_indicator_handle_t &handle,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
) {
    if (attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
        return app_driver_light_set_brightness(handle, val);
    }

    return ESP_OK;
}

static esp_err_t handle_color_control_attribute_update(
    led_indicator_handle_t &handle,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
) {
    switch (attribute_id) {
        case ColorControl::Attributes::CurrentHue::Id:
            return app_driver_light_set_hue(handle, val);
        case ColorControl::Attributes::CurrentSaturation::Id:
            return app_driver_light_set_saturation(handle, val);
        case ColorControl::Attributes::ColorTemperatureMireds::Id:
            return app_driver_light_set_temperature(handle, val);
        default:
            return ESP_OK;
    }
}

esp_err_t handle_light_attribute_update(
    app_driver_handle_t driver_handle,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
) {
    led_indicator_handle_t handle = (led_indicator_handle_t)driver_handle;
    switch (cluster_id) {
        case OnOff::Id:
            return handle_on_off_attribute_update(handle, endpoint_id, cluster_id, attribute_id, val);
        case LevelControl::Id:
            return handle_level_control_attribute_update(handle, endpoint_id, cluster_id, attribute_id, val);
        case ColorControl::Id:
            return handle_color_control_attribute_update(handle, endpoint_id, cluster_id, attribute_id, val);
        default:
            return ESP_OK;
    }
}

endpoint_descriptor_t light_endpoint_create_descriptor() {
    endpoint_descriptor_t descriptor = {
        .handle = (light_endpoint_t) malloc(sizeof(_light_endpoint_t)),
        .endpoint_id = 0,
        .register_endpoint = register_light_endpoint,
        .on_start = light_endpoint_on_start,
        .handle_attribute_update = handle_light_attribute_update
    };

    return descriptor;
}
