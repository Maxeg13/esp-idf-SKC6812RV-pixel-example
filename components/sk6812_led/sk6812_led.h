#include "stdint.h"

//  add to gpio.c:
//  esp_err_t gpio_set_level_insecure(gpio_num_t gpio_num, uint32_t level)
//  {
//      GPIO_CHECK((gpio_num >= 0), "GPIO output gpio_num error", ESP_ERR_INVALID_ARG);
//      gpio_hal_set_level(gpio_context.gpio_hal, gpio_num, level);
//      return ESP_OK;
// }

void sk6812_ledInit();

struct ColourState {
    static constexpr uint8_t step = 4;

    uint8_t g;
    uint8_t r;
    uint8_t b;

    void stepTo(ColourState& targ);
};

void sk6812Shine(const ColourState& state);