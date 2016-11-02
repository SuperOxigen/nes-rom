#pragma once

#include "sr_button.hpp"

class ControlPanel
{
    pin_t    _led_a;
    SrButton _btn_a;
    pin_t    _led_b;
    SrButton _btn_b;


public:
    ControlPanel(pin_t led_a, pin_t in_a, pin_t res_a,
                 pin_t led_b, pin_t in_b, pin_t res_b);

    void init_pins(void);

    void led_a_on();
    void led_a_off();
    int read_a();
    int poll_a() const;
    void reset_a();

    pin_t led_a_pin() const;
    pin_t in_a_pin() const;
    pin_t res_a_pin() const;

    void led_b_on();
    void led_b_off();
    int read_b();
    int poll_b() const;
    void reset_b();

    pin_t led_b_pin() const;
    pin_t in_b_pin() const;
    pin_t res_b_pin() const;
};
