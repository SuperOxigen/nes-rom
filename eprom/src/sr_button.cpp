#include <Arduino.h>
#include "sr_button.hpp"

#define PULSE_DELAY 3
#define DEBOUNCE_DELAY 25

SrButton::SrButton(pin_t in, pin_t reset):
    _in(in), _res(reset)
{
    this->init_pins();
    this->reset();
}

void SrButton::init_pins()
{
    pinMode(this->_in, INPUT);
    pinMode(this->_res, OUTPUT);
}

int SrButton::poll() const
{
    return (digitalRead(this->_in) == HIGH);
}

void SrButton::reset()
{
    digitalWrite(this->_res, HIGH);
    delay(PULSE_DELAY);
    digitalWrite(this->_res, LOW);
    delay(PULSE_DELAY);
}

int SrButton::read()
{
    int val;
    do
    {
        val = this->poll();
        if (val) this->reset();
        delay(DEBOUNCE_DELAY);
    } while(val && this->poll());

    return val;
}

pin_t SrButton::in_pin() const
{
    return this->_in;
}

pin_t SrButton::res_pin() const
{
    return this->_res;
}
