#pragma once

#include "utils.h"

class SrButton
{
    pin_t   _in,
            _res;

    void init_pins(void);
public:
    SrButton(pin_t in, pin_t reset);

    int poll(void) const;
    void reset(void);
    int read(void);

    pin_t in_pin(void) const;
    pin_t res_pin(void) const;
};
