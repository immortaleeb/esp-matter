#define LIGHT_MODE_BUILTIN_LED 0
#define LIGHT_MODE_SINGLE_LED 1
#define LIGHT_MODE_SIMPLE_RGB_STRIP 2

#define LIGHT_MODE LIGHT_MODE_SIMPLE_RGB_STRIP

#if LIGHT_MODE == LIGHT_MODE_BUILTIN_LED
#include <light_builtin.cpp>
#elif LIGHT_MODE == LIGHT_MODE_SINGLE_LED
#include <light_single_led.cpp>
#elif LIGHT_MODE == LIGHT_MODE_SIMPLE_RGB_STRIP
#include <light_simple_rgb_strip.cpp>
#endif
