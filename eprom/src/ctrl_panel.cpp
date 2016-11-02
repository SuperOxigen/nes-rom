#include <Arduino.h>
#include "ctrl_panel.hpp"

#define PULSE_DELAY 3

ControlPanel::ControlPanel(pin_t led_a, pin_t in_a, pin_t res_a,
                           pin_t led_b, pin_t in_b, pin_t res_b):
    _led_a(led_a), _btn_a{in_a, res_a},
    _led_b(led_b), _btn_b{in_b, res_b}
{
    this->init_pins();
    this->led_a_off();
    this->led_b_off();
}

void ControlPanel::init_pins()
{
    pinMode(this->_led_a, OUTPUT);
    pinMode(this->_led_b, OUTPUT);
    digitalWrite(this->_led_a, LOW);
    digitalWrite(this->_led_b, LOW);
}

void ControlPanel::led_a_on()
{
    digitalWrite(this->_led_a, HIGH);
}

void ControlPanel::led_a_off()
{
    digitalWrite(this->_led_a, LOW);
}

int ControlPanel::read_a()
{
    return this->_btn_a.read();
}

int ControlPanel::poll_a() const
{
    return this->_btn_a.poll();
}

void ControlPanel::reset_a()
{
    return this->_btn_a.reset();
}

pin_t ControlPanel::led_a_pin() const
{
    return this->_led_a;
}

pin_t ControlPanel::in_a_pin() const
{
    return this->_btn_a.in_pin();
}

pin_t ControlPanel::res_a_pin() const
{
    return this->_btn_a.res_pin();
}

/* Set B */

void ControlPanel::led_b_on()
{
    digitalWrite(this->_led_b, HIGH);
}

void ControlPanel::led_b_off()
{
    digitalWrite(this->_led_b, LOW);
}

int ControlPanel::read_b()
{
    return this->_btn_b.read();
}

int ControlPanel::poll_b() const
{
    return this->_btn_b.poll();
}

void ControlPanel::reset_b()
{
    return this->_btn_b.reset();
}

pin_t ControlPanel::led_b_pin() const
{
    return this->_led_b;
}

pin_t ControlPanel::in_b_pin() const
{
    return this->_btn_b.in_pin();
}

pin_t ControlPanel::res_b_pin() const
{
    return this->_btn_b.res_pin();
}
