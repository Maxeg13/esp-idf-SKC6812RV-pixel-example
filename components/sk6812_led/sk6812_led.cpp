#include "sk6812_led.h"
#include "driver/gpio.h"

#define GPIO_OUT GPIO_NUM_32

static void set_t0h() {
    gpio_set_level_insecure(GPIO_OUT, 1);
}

static void set_t0l() {
    gpio_set_level_insecure(GPIO_OUT, 0);
    gpio_set_level_insecure(GPIO_OUT, 0);
    gpio_set_level_insecure(GPIO_OUT, 0);
}

static void set_t1h() {
    gpio_set_level_insecure(GPIO_OUT, 1);
    gpio_set_level_insecure(GPIO_OUT, 1);
}

static void set_t1l() {
    gpio_set_level_insecure(GPIO_OUT, 0);
    gpio_set_level_insecure(GPIO_OUT, 0);
}

static void add(uint8_t& x, uint8_t incr) {
    if(x + incr > 255) x = 255;
    else x+= incr;
}

static void minus(uint8_t& x, uint8_t decr) {
    if(x - decr > x) x = 0;
    else x -= decr;
}

void ColourState::print() const{
    printf("(%d, %d, %d)\n", g, r, b);
}

void ColourState::initTarget(const ColourState* p) const {
    targetPtr = const_cast<ColourState*>(p);
}

////////////////

void skc6812_led_Init() {
    gpio_reset_pin(GPIO_OUT);
    gpio_set_direction(GPIO_OUT, GPIO_MODE_OUTPUT);
}

void ColourState::stepTo(const ColourState &targ) {
    if(targ.g > g)      add(g, step);
    else if(g > step)   minus(g, step);

    if(targ.r > r)      add(r, step);
    else if(r > step)   minus(r, step);

    if(targ.b > b)      add(b, step);
    else if(b > step)   minus(b, step);
}

void skc6812_shine(const ColourState& state) {
    static uint8_t bits[24]{};
    const uint8_t* colourPtrs[3] = {&state.g, &state.r, &state.b};

    for(int i=0; const auto& colourPtr: colourPtrs) {
        for(int b = 7; (i < sizeof(bits)) && b != -1; i++, b--) {
            bits[i] = (*colourPtr >> b) & 0x1;
        }
    }

    for(int i=0; i < sizeof(bits); i++) {
        if(bits[i]) {
            set_t1h();
            set_t1l();
        } else {
            set_t0h();
            set_t0l();
        }
    }

    gpio_set_level_insecure(GPIO_OUT, 0);
}

void skc6812_blue_test() {
    int i = 0;
    //  green
    for( i=0; i<8; i++) {
//        set_t1h();
//        set_t1l();
        set_t0h();
        set_t0l();
    }

    //  red
    for(; i<16; i++) {
    //    set_t1h();
    //    set_t1l();
        set_t0h();
        set_t0l();
    }

    // blue
    for(; i<24; i++) {
        set_t1h();
        set_t1l();
//        set_t0h();
//        set_t0l();
    }
    set_t0l();
}