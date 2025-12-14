#include "sk6812_led.h"
#include "driver/gpio.h"

#define GPIO_OUT GPIO_NUM_32

void sk6812_ledInit() {
    gpio_reset_pin(GPIO_OUT);
    gpio_set_direction(GPIO_OUT, GPIO_MODE_OUTPUT);
}

void setT0H() {
    gpio_set_level_insecure(GPIO_OUT, 1);
}

void setT0L() {
    gpio_set_level_insecure(GPIO_OUT, 0);
    gpio_set_level_insecure(GPIO_OUT, 0);
    gpio_set_level_insecure(GPIO_OUT, 0);
}

void setT1H() {
    gpio_set_level_insecure(GPIO_OUT, 1);
    gpio_set_level_insecure(GPIO_OUT, 1);
}

void setT1L() {
    gpio_set_level_insecure(GPIO_OUT, 0);
    gpio_set_level_insecure(GPIO_OUT, 0);
}

void add(uint8_t& x, uint8_t incr) {
    if(x + incr > 255) x = 255;
    else x+= incr;
}

void minus(uint8_t& x, uint8_t decr) {
    if(x - decr > x) x = 0;
    else x -= decr;
}

void ColourState::stepTo(ColourState &targ) {
    if(targ.g > g)      add(g, step);
    else if(g > step)   minus(g, step);

    if(targ.r > r)      add(r, step);
    else if(r > step)   minus(r, step);

    if(targ.b > b)      add(b, step);
    else if(b > step)   minus(b, step);
}

void sk6812Shine(const ColourState& state) {
    static uint8_t bits[24]{};
    const uint8_t* colourPtrs[3] = {&state.g, &state.r, &state.b};

    for(int i=0; const auto& colourPtr: colourPtrs) {
        for(int b = 7; (i < sizeof(bits)) && b != -1; i++, b--) {
            bits[i] = (*colourPtr >> b) & 0x1;
        }
    }

    for(int i=0; i < sizeof(bits); i++) {
        if(bits[i]) {
            setT1H();
            setT1L();
        } else {
            setT0H();
            setT0L();
        }
    }

    gpio_set_level_insecure(GPIO_OUT, 0);
}